#ifndef MODEL_H
#define MODEL_H

#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "stb_image.h"


unsigned int TextureFromFile(const char* path, std::string& directory, bool gamma = false);

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;
	//Vertex myPosition;

	Model(){
	}

	Model(std::string const & path, bool gamma = false) : gammaCorrection(gamma)
	{
		this->loadModel(path);
		std::cout << "obj-File loaded" << std::endl;
	}

	/** Draw the model */
	void draw(Shader shader);

private:

	/** load the Model with assimp from obj-file */
	void loadModel(std::string path);

	/** process nodes recursive */
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	/** Checks all material textures of a given type and loads the textures */
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif