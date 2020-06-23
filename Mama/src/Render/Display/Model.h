#ifndef MODEL_H
#define MODEL_H

#pragma once
#include "Mesh.h"
#include "Shader.h"



unsigned int TextureFromFile(const char* path, std::string& directory, bool gamma = false);

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
	glm::vec3 GetPosition() { return m_Position; }
	glm::vec3 GetMinPos() { return m_MinPos; }
	glm::vec3 GetMaxPos() { return m_MaxPos; }

	/** Draw the model */
	void draw(Shader shader);

private:
	glm::vec3 m_Position;
	glm::vec3 m_MinPos;
	glm::vec3 m_MaxPos;

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