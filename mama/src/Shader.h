#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

//------------------------------------------------------------------struct definitions-------------------------------------------------------------------
struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------

class Shader
{
	/* Varables */
public:
	ShaderSource source;
	unsigned int ID;
	unsigned int program;

	/* Functions */
public:
	Shader(const std::string& filePath);
	void use() const;
	void deleteShader() const;
	bool shaderGeometry;

	/* utility */
	// utility functions defined in ShaderUtility.cpp
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float a) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	/* Functions */
private:
	/** devides a given filepath into VERTEX and FRAGMENT shader */
	ShaderSource parseShader(const std::string& filePath);
	int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	int createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
}; 
