#ifndef MODEL_H
#define MODEL_H

#pragma once
#include "Mesh.h"
#include "Shader.h"

#include <algorithm>
#define _DEBUG_LT_PRED



unsigned int TextureFromFile(const char* path, std::string& directory, bool gamma = false);
struct compareVertex {
	bool operator()( Vertex &left,  Vertex &right) const
	{
		return left.position.x < right.position.x && left.position.y < right.position.y && left.position.z < right.position.z;
	}
};

class Model
{
public:
	int getFaceCount() { return faceCount; }

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;
	//Vertex myPosition;

	Model(){}

	Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		m_MinPos = glm::vec3(FLT_MAX);
		m_MaxPos = glm::vec3(-FLT_MAX);
		this->loadModel(path);
		std::cout << "obj-File " << path << " loaded" << std::endl;
	}
	float GetDistance() { return (float)sqrt(pow(m_MaxPos.x - m_MinPos.x, 2) + pow(m_MaxPos.y - m_MinPos.y, 2) + pow(m_MaxPos.z - m_MinPos.z, 2)); }
	glm::vec3 GetDistanceVec() { return m_MaxPos - m_MinPos; }

	glm::vec3 GetPosition() { return m_Position; }
	glm::vec3 GetMinPos() { return m_MinPos; }
	glm::vec3 GetMaxPos() { return m_MaxPos; }
	void setTransform(glm::mat4 transform) { m_Transform = transform; }
	void makeDynamic() { m_Dynamic = true; }

	glm::vec3 GetCenter()
	{
		//if (m_Dynamic)
			//calculateMinMax();

		glm::vec3 d = GetDistanceVec();
		return GetMinPos() + d/2.0f;
	}

	/** Draw the model */
	void draw(Shader shader);

private:
	glm::vec3 m_Position;
	glm::vec3 m_MinPos;
	glm::vec3 m_MaxPos;

	glm::mat4 m_Transform = glm::mat4(1.0f);

	bool m_Dynamic = false;

	int faceCount;

	/** load the Model with assimp from obj-file */
	void loadModel(std::string path);

	/** process nodes recursive */
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	/** Checks all material textures of a given type and loads the textures */
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void calculateMinMax() {
		std::vector<glm::vec3> newVerts;
		for (Vertex v : meshes[0].vertices) {
			newVerts.push_back(m_Transform * glm::vec4(v.position, 1));
		}

		m_MinPos = glm::vec3(FLT_MAX);
		m_MaxPos = glm::vec3(-FLT_MAX);
		
		for (glm::vec3 vector : newVerts) {
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
	}
};


#endif