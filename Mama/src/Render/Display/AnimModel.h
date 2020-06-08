#pragma once
#include "AnimMesh.h"
#include "Shader.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <map>

uint32_t TextureFromAnimFile(const char* path, std::string& directory, bool gamma = false);

class AnimModel
{
public:
	static const uint32_t MAX_BONES = 100;
	bool m_Gamma;

	AnimModel() = default;
	AnimModel(std::string const &path, bool gamma = false);

	float GetDistance() { return (float)sqrt(pow(m_MaxPos.x - m_MinPos.x, 2) + pow(m_MaxPos.y - m_MinPos.y, 2) + pow(m_MaxPos.z - m_MinPos.z, 2)); }
	glm::vec3 GetPosition() { return m_Position; }

	/** Draw the model */
	void Draw(Shader shader);

	void InitShader(Shader &shader);

private:
	std::vector<AnimTexture> m_LoadedTex;
	std::vector<AnimMesh> m_Meshes;
	std::vector<BoneMatrix> m_Bones;

	glm::vec3 m_Position;
	glm::vec3 m_MinPos;
	glm::vec3 m_MaxPos;

	std::map<std::string, uint32_t> m_BoneMap;

	Assimp::Importer m_Importer;
	const aiScene* m_Scene;
	aiMatrix4x4 m_Global_Inverse_Transformation_Matrix;

	std::string m_Dir;
	float m_TicksPerSecond = 0.0f;
	uint32_t m_BoneNumber = 0;
	GLuint m_BonePosition[MAX_BONES];

	void ShowNodeNames(aiNode* node);

	uint32_t GetPosition(float time, const aiNodeAnim* nodeAnim);
	uint32_t GetRotation(float time, const aiNodeAnim* nodeAnim);
	uint32_t GetScale(float time, const aiNodeAnim* nodeAnim);

	glm::vec3 GetMax() { return m_MaxPos; }

	/** load the Model with assimp from obj-file */
	void loadModel(std::string path);

	/** process nodes recursive */
	void processNode(aiNode* node, const aiScene* scene);
	AnimMesh processMesh(aiMesh* mesh, const aiScene* scene);

	aiVector3D calcPosition(float time, const aiNodeAnim* anim);
	aiVector3D calcScale(float time, const aiNodeAnim* anim);
	aiQuaternion calcRotation(float time, const aiNodeAnim* anim);

	const aiNodeAnim* getAnim(const aiAnimation* anim, const std::string name);
	void readNodeHierarchy(float time, const aiNode* node, const aiMatrix4x4 parent);
	void BoneTransformation(double timeInSec, std::vector<aiMatrix4x4> &transforms);

	glm::mat4 aiMatToGlmMat(aiMatrix4x4 mat);
	aiQuaternion Nlerp(aiQuaternion a, aiQuaternion b, float factor);
	/** Checks all material textures of a given type and loads the textures */
	std::vector<AnimTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

