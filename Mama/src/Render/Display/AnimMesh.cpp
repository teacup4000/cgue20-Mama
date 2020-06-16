#include "stdafx.h"
#include "AnimMesh.h"
								////////////////////////////////////////////////////////////////
								// THE MESH CLASS FOR ANIMATIONS							  //
								// This class stores the data from the animation file inside  //
								// specific vertex, index and array buffers.				  //
								////////////////////////////////////////////////////////////////

void VertexBoneData::addBoneData(uint32_t boneID, float weight)
{
	for (uint32_t i = 0; i < NUM_BONES_PER_VERTEX; i++)
	{
		if (weights[i] == 0.0)
		{
			IDs[i] = boneID;
			weights[i] = weight;
			return;
		}
	}
}

AnimMesh::AnimMesh(std::vector<AnimVertex> vertices, std::vector<GLuint> indices, std::vector<AnimTexture> textures, std::vector<VertexBoneData> weightsPerVertex)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;
	m_WeightsPerVertex = weightsPerVertex;

	SetupAnimMesh();
}

void AnimMesh::Draw(Shader shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < m_Textures.size(); i++)
	{
		/* Active a texture unit before binding */
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream sStream;
		std::string number;
		std::string name = m_Textures[i].type;
		if (name == "texture_diffuse")
			sStream << diffuseNr++;
		else if (name == "texture_specular")
			sStream << specularNr++;

		number = sStream.str();

		/* bind the texture */
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
	}

	/** Draw mesh */
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	/* setting all to default */
	for (GLuint i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void AnimMesh::SetupAnimMesh()
{
	//vertex data
	glGenBuffers(1, &VBO_Vert);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(m_Vertices[0]), &m_Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones data
	glGenBuffers(1, &VBO_Bone);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Bone);
	glBufferData(GL_ARRAY_BUFFER, m_WeightsPerVertex.size() * sizeof(m_WeightsPerVertex[0]), &m_WeightsPerVertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//bind buffer to shader
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vert);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)0);
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Bone);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); 
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
	
}