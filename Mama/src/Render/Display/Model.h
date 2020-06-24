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


	glm::vec3 GetCenter()
	{
		glm::vec3 d = GetDistanceVec();
		return GetMinPos() + d/2.0f;
	}

	void GetSortedVertices()
	{
		std::sort(m_Center.begin(), m_Center.end(), compareVertex());
		for (int i = 0; i < m_Center.size(); i++)
		{
			std::cout << m_Center[i].position.x << ", " << m_Center[i].position.y << ", " << m_Center[i].position.z << std::endl;
		}
	}

	/** Draw the model */
	void draw(Shader shader);

private:
	glm::vec3 m_Position;
	glm::vec3 m_MinPos;
	glm::vec3 m_MaxPos;
	std::vector<Vertex> m_Center;

	glm::vec3 GetMax() { return m_MaxPos; }

	int faceCount;

	/** load the Model with assimp from obj-file */
	void loadModel(std::string path);

	/** process nodes recursive */
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	/** Checks all material textures of a given type and loads the textures */
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif