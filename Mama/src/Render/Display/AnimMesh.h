#pragma once
#include "Shader.h"
#include "Core/Core.h"

#include <string>
#include <iostream>
#include <vector>

//assimp library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct AnimVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct AnimTexture {
	GLuint id;
	std::string type;
	aiString path;
};

struct VertexBoneData
{
	uint32_t IDs[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		ZERO_MEM(IDs);
		ZERO_MEM(weights);
	}

	void addBoneData(uint32_t boneID, float weight);
};

struct BoneMatrix
{
	aiMatrix4x4 BoneOffset;
	aiMatrix4x4 FinalTransformationMatrix;
};
class AnimMesh
{
public:
	AnimMesh() = default;
	AnimMesh(std::vector<AnimVertex> vertices, std::vector<GLuint> indices, std::vector<AnimTexture> textures, std::vector<VertexBoneData> weightsPerVertex);
	void Draw(Shader shader);

private:
	std::vector<AnimVertex> m_Vertices;
	std::vector<GLuint> m_Indices;
	std::vector<AnimTexture> m_Textures;
	std::vector<VertexBoneData> m_WeightsPerVertex;

	GLuint VAO, VBO_Vert,VBO_Bone, EBO;

	void SetupAnimMesh();
};

