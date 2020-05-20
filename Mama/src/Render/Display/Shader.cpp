#include "stdafx.h"
//Discription of functions in *.h files
#include "Shader.h"



//---------------------------------------------------only Shader intern functoins in this cpp------------------------------------------------------------
Shader::Shader(const std::string& filePath)
{
	source = parseShader(filePath);
	if (shaderGeometry) {
		ID = createShader(source.vertexSource, source.fragmentSource, source.geometrySource);
	}
	else {
		ID = createShader(source.vertexSource, source.fragmentSource);
	}
	use();
}

ShaderSource Shader::parseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};

	std::string line;
	std::stringstream shaderStream[3];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				/* set modus to vertex */
				type = ShaderType::VERTEX;
				std::cout << "VERTEX Shader detected" << std::endl;
				shaderGeometry = false;

			}
			else if (line.find("fragment") != std::string::npos)
			{
				/* set modus to fragment */
				type = ShaderType::FRAGMENT;
				std::cout << "FRAGMENT Shader detected" << std::endl;
				shaderGeometry = false;
			}
			else if (line.find("geometry") != std::string::npos)
			{
				type = ShaderType::GEOMETRY;
				std::cout << "GEOMETRY Shader detected" << std::endl;
				shaderGeometry = true;
			}
		}
		else
		{
			/* insert lines into stream */
			shaderStream[(int)type] << line << '\n';
		}
	}
	/* return the struct */
	return { shaderStream[0].str(), shaderStream[1].str(), shaderStream[2].str() };
}

int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

/** If ever there needs to be some depth information, then this shader is called. It uses an additional geometry shader, that calculates the depth and it is used for shadow mapping/light mapping
This code could still be useful for later work! Do not delete it. */

int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glAttachShader(program, gs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);

	return program;
}


unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
	ID = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(ID, 1, &src, nullptr);
	glCompileShader(ID);

	int result;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(ID, length, &length, msg);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : "geometry") << " shader!" << std::endl;
		std::cout << msg << std::endl;
		glDeleteShader(ID);
		return 0;
	}
	return ID;
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::deleteShader() const
{
	glDeleteProgram(ID);
}