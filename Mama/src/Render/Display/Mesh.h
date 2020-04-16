#pragma once
#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>

//assimp library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

class Mesh {
public: //MEMBERS
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

public: //FUNCTIONS
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	void draw(Shader shader);

private: //MEMBERS
	GLuint VAO, VBO, EBO;

private: //FUNCTIONS
	void setupMesh();
};



