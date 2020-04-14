#pragma once
#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>

//assimp library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//----------------------------------------------------------------struct definitions--------------------------------------------------------------------
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 biTangent;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------

class Mesh {
	//Variables 
public:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	//Functions
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	/** Rendering the Mesh */
	void draw(Shader shader);

	//Variables
private:
	GLuint VAO, VBO, EBO;

	//Functions
private:
	/** Initialize all Buffers and Arrays */

	//function defined in Renderer.cpp
	void setupMesh();
};



