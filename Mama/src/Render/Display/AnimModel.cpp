#include "stdafx.h"
#include "AnimModel.h"

#include <GLFW/glfw3.h>

#include "stb_image.h"

								////////////////////////////////////////////////////////////////
								// THE ANIMATION CLASS										  //
								// This class loads an animation file via the assimp library  //
								// and renders it onto the screen.							  //
								////////////////////////////////////////////////////////////////
              
AnimModel::AnimModel(std::string const &path, bool gamma)
	:m_Gamma(gamma)
{
	m_Scene = nullptr;
	loadModel(path);
	std::cout << "anim-File loaded" << std::endl;
}

void AnimModel::InitShader(Shader &shader)
{
	for (uint32_t i = 0; i < MAX_BONES; i++)
	{
		std::string name = "bones[" + std::to_string(i) + "]";
		m_BonePosition[i] = glGetUniformLocation(shader.ID, name.c_str());
	}
}

void AnimModel::Draw(Shader shader)
{
	std::vector<aiMatrix4x4> transformationMatrix;
	BoneTransformation((double)glfwGetTime(), transformationMatrix);

	for (uint32_t i = 0; i < transformationMatrix.size(); i++)
		glUniformMatrix4fv(m_BonePosition[i], 1, GL_TRUE, (const GLfloat*)&transformationMatrix[i]);

	for (GLuint i = 0; i < m_Meshes.size(); i++)
		m_Meshes[i].Draw(shader);
}

void AnimModel::loadModel(std::string path)
{
	/* load and read obj-file */
	m_Scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << m_Importer.GetErrorString() << std::endl;
		return;
	}

	m_Global_Inverse_Transformation_Matrix = m_Scene->mRootNode->mTransformation;
	m_Global_Inverse_Transformation_Matrix.Inverse();

	if (m_Scene->mAnimations[0]->mTicksPerSecond != 0.0f)
		m_TicksPerSecond = m_Scene->mAnimations[0]->mTicksPerSecond;
	else
		m_TicksPerSecond = 25.0f;

	/* Retrieve the directory path of the filepath */
	 m_Dir = path.substr(0, path.find_last_of('/'));

	 std::cout << "The following nodes have been found: " << std::endl;
	 processNode(m_Scene->mRootNode, m_Scene);
}

void AnimModel::ShowNodeNames(aiNode* node)
{
	std::cout << node->mName.data << std::endl;
	for (uint32_t i = 0; i < node->mNumChildren; i++)
		ShowNodeNames(node->mChildren[i]);
}

void AnimModel::processNode(aiNode* node, const aiScene* scene)
{
	/* Process each mesh at the current node */
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		 m_Meshes.push_back( processMesh(mesh, scene));
	}
	/* process each of the children nodes */
	for (GLuint i = 0; i < node->mNumChildren; i++)
		 processNode(node->mChildren[i], scene);
}

AnimMesh AnimModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AnimVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<AnimTexture> textures;
	std::vector<VertexBoneData> weightsPerVertex;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumVertices);
	weightsPerVertex.resize(mesh->mNumVertices);

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		AnimVertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		{
			//MIN
			if (vector.x < m_MinPos.x)
				m_MinPos.x = vector.x;
			if (vector.y < m_MinPos.y)
				m_MinPos.y = vector.y;
			if (vector.z < m_MinPos.z)
				m_MinPos.z = vector.z;

			//MAX
			if (vector.x > m_MaxPos.x)
				m_MaxPos.x = vector.x;
			if (vector.y > m_MaxPos.y)
				m_MaxPos.y = vector.y;
			if (vector.z > m_MaxPos.z)
				m_MaxPos.z = vector.z;
		}

		vertex.position = vector;
		m_Position = vector;

		if (mesh->mNormals != NULL) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		else
		{
			vertex.normal = glm::vec3();
		}

		/* check if mesh contains texCoords */
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);

	}
	/* Process the materials */
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. Diffuse maps
		std::vector<AnimTexture> diffuseMaps =  loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		std::vector<AnimTexture> specularMaps =  loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	/* load Bones */
	for (uint32_t i = 0; i < mesh->mNumBones; i++)
	{
		uint32_t index = 0;
		std::string name(mesh->mBones[i]->mName.data);
		std::cout << "        " + name << std::endl;

		if (m_BoneMap.find(name) == m_BoneMap.end())
		{
			index = m_BoneNumber;
			m_BoneNumber++;
			BoneMatrix bones;
			m_Bones.push_back(bones);

			m_Bones[index].BoneOffset = mesh->mBones[i]->mOffsetMatrix;
			m_BoneMap[name] = index;
		}
		else
			index = m_BoneMap[name];

		for (uint32_t j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			uint32_t vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float currentWeight = mesh->mBones[i]->mWeights[j].mWeight;
			weightsPerVertex[vertexID].addBoneData(index, currentWeight);
		}
	}
	return AnimMesh(vertices, indices, textures, weightsPerVertex);
}

uint32_t AnimModel::GetPosition(float time, const aiNodeAnim* nodeAnim)
{
	for (uint32_t i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (time < (float)nodeAnim->mPositionKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

uint32_t AnimModel::GetRotation(float time, const aiNodeAnim* nodeAnim)
{
	for (uint32_t i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (time < (float)nodeAnim->mRotationKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

uint32_t AnimModel::GetScale(float time, const aiNodeAnim* nodeAnim)
{
	for (uint32_t i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (time < (float)nodeAnim->mScalingKeys[i + 1].mTime)
			return i;
	}

	assert(0);
	return 0;
}

aiVector3D AnimModel::calcPosition(float time, const aiNodeAnim* anim)
{
	if (anim->mNumPositionKeys == 1)
		return anim->mPositionKeys[0].mValue;

	uint32_t index = GetPosition(time, anim);
	uint32_t next = index + 1;
	assert(next < anim->mNumPositionKeys);

	float deltaTime = (float)(anim->mPositionKeys[next].mTime - anim->mPositionKeys[index].mTime);
	float interpolationRatio = (time - (float)anim->mPositionKeys[index].mTime) / deltaTime;
	assert(interpolationRatio >= 0.0f && interpolationRatio <= 1.0f);

	aiVector3D start = anim->mPositionKeys[index].mValue;
	aiVector3D end = anim->mPositionKeys[next].mValue;
	
	return start + interpolationRatio * (end - start);
}

aiVector3D AnimModel::calcScale(float time, const aiNodeAnim* anim)
{
	if (anim->mNumScalingKeys == 1)
		return anim->mScalingKeys[0].mValue;

	uint32_t index = GetScale(time, anim);
	uint32_t next = index + 1;
	assert(next < anim->mNumScalingKeys);

	float deltaTime = (float)(anim->mScalingKeys[next].mTime - anim->mScalingKeys[index].mTime);
	float interpolationRatio = (time - (float)anim->mScalingKeys[index].mTime) / deltaTime;
	assert(interpolationRatio >= 0.0f && interpolationRatio <= 1.0f);

	aiVector3D start = anim->mScalingKeys[index].mValue;
	aiVector3D end = anim->mScalingKeys[next].mValue;
	 

	return start + interpolationRatio * (end - start);
}

aiQuaternion AnimModel::calcRotation(float time, const aiNodeAnim* anim)
{
	if (anim->mNumRotationKeys == 1)
		return anim->mRotationKeys[0].mValue;

	uint32_t index = GetRotation(time, anim);
	uint32_t next = index + 1;
	assert(next < anim->mNumRotationKeys);

	float deltaTime = (float)(anim->mRotationKeys[next].mTime - anim->mRotationKeys[index].mTime);
	float interpolationRatio = (time - (float)anim->mRotationKeys[index].mTime) / deltaTime;
	assert(interpolationRatio >= 0.0f && interpolationRatio <= 1.0f);

	aiQuaternion start = anim->mRotationKeys[index].mValue;
	aiQuaternion end = anim->mRotationKeys[next].mValue;

	return Nlerp(start, end, interpolationRatio);
}

aiQuaternion AnimModel::Nlerp(aiQuaternion a, aiQuaternion b, float ratio)
{
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float oneMinusRatio = 1.0f - ratio;

	if (dot < 0.0f)
	{
		result.x = a.x * oneMinusRatio + ratio * -b.x;
		result.y = a.y * oneMinusRatio + ratio * -b.y;
		result.z = a.z * oneMinusRatio + ratio * -b.z;
		result.w = a.w * oneMinusRatio + ratio * -b.w;
	}
	else
	{
		result.x = a.x * oneMinusRatio + ratio * b.x;
		result.y = a.y * oneMinusRatio + ratio * b.y;
		result.z = a.z * oneMinusRatio + ratio * b.z;
		result.w = a.w * oneMinusRatio + ratio * b.w;
	}
	return result.Normalize();
}

const aiNodeAnim* AnimModel::getAnim(const aiAnimation*  anim, const std::string name)
{
	for (uint32_t i = 0; i < anim->mNumChannels; i++)
	{
		const aiNodeAnim* nodeAnim = anim->mChannels[i];
		if (std::string(nodeAnim->mNodeName.data) == name)
			return nodeAnim;
	}
	return nullptr;
}

void AnimModel::readNodeHierarchy(float time, const aiNode* node, const aiMatrix4x4 parent)
{
	std::string name(node->mName.data);

	const aiAnimation* anim = m_Scene->mAnimations[0];
	aiMatrix4x4 localTransformMatrix = node->mTransformation;

	const aiNodeAnim* nodeAnim = getAnim(anim, name);

	if (nodeAnim)
	{
		//translation
		aiVector3D transVec = calcPosition(time, nodeAnim);
		aiMatrix4x4 transMat;
		aiMatrix4x4::Translation(transVec, transMat);

		//rotation
		aiQuaternion rotateData = calcRotation(time, nodeAnim);
		aiMatrix4x4 rotateMat = aiMatrix4x4(rotateData.GetMatrix());

		//scale
		aiVector3D scaleVec = calcScale(time, nodeAnim);
		aiMatrix4x4 scaleMat;
		aiMatrix4x4::Scaling(scaleVec, scaleMat);

		localTransformMatrix = transMat * rotateMat * scaleMat;
	}
	aiMatrix4x4 globalTransformMatrix = parent * localTransformMatrix;

	if (m_BoneMap.find(name) != m_BoneMap.end())
	{
		uint32_t index = m_BoneMap[name];
		m_Bones[index].FinalTransformationMatrix =	m_Global_Inverse_Transformation_Matrix * 
													globalTransformMatrix * 
													m_Bones[index].BoneOffset;
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
		readNodeHierarchy(time, node->mChildren[i], globalTransformMatrix);
}

void AnimModel::BoneTransformation(double timeInSec, std::vector<aiMatrix4x4> &transformationMatrix)
{
	aiMatrix4x4 identity;

	double timeInTicks = timeInSec * m_TicksPerSecond;
	float animTime = fmod(timeInTicks, m_Scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animTime, m_Scene->mRootNode, identity);
	transformationMatrix.resize(m_BoneNumber);

	for (uint32_t i = 0; i < m_BoneNumber; i++)
		transformationMatrix[i] = m_Bones[i].FinalTransformationMatrix;

}

std::vector<AnimTexture> AnimModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<AnimTexture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		/* Check if texture was loaded before */
		GLboolean skip = false;
		for (GLuint j = 0; j < m_LoadedTex.size(); j++)
		{
			if (std::strcmp(m_LoadedTex[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			AnimTexture texture;
			texture.id = TextureFromAnimFile(str.C_Str(),  m_Dir);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_LoadedTex.push_back(texture);
		}
	}
	return textures;
}


/** load the texture from a File (PNG, JPG) */
unsigned int TextureFromAnimFile(const char* path, std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//PFNGLGENERATEMIPMAPPROC my_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glfwGetProcAddress("glGenerateMipmap");
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load for path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void renderAnimModel(AnimModel &model, Shader &shader, glm::mat4 matrix)
{
	shader.setMat4("model", matrix);
	model.Draw(shader);
}

