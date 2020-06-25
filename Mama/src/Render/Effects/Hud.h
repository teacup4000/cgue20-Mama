#pragma once
#include <cstdio>
#include <iostream>
#include <glm.hpp>
#include <list>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>

#include "Render/Display/stb_image.h"
#include "Render/Display/Shader.h"
#include "Render/Renderer.h"

#include <Windows.h>

extern void renderModel(Model &model, Shader &shader, glm::mat4 matrix);

class GUITex
{
public:
	GUITex(uint32_t tex, glm::vec2 pos, glm::vec2 scale)
		:m_Tex(tex), m_Pos(pos), m_Scale(scale) {}

	uint32_t GetTex() { return m_Tex; }
	glm::vec2 GetPos() { return m_Pos; }
	glm::vec2 GetScale() { return m_Scale; }

private:
	uint32_t m_Tex;
	glm::vec2 m_Pos;
	glm::vec2 m_Scale;
};

class QuadModel
{
public:
	QuadModel() = default;
	QuadModel(uint32_t vaoID, int vertexCount)
		:m_VaoID(vaoID), m_VertexCount(vertexCount) {}

	uint32_t getVaoID() { return m_VaoID; }
	uint32_t getVertexCount() { return m_VertexCount; }
private:
	uint32_t m_VaoID;
	uint32_t m_VertexCount;
};

class Loader
{
public:
	QuadModel loadToVAO(std::vector<float> positions)
	{
		uint32_t vaoID = createVAO();
		storeDataInAttributeList(0, positions);
		unbindVAO();
		QuadModel model(vaoID, positions.size() / 2);
		return model;
	}

	uint32_t createVAO()
	{
		uint32_t vaoID;
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		vaos.push_back(vaoID);
		return vaoID;

	}

	void storeDataInAttributeList(uint32_t attribNr, std::vector<float> data)
	{
		uint32_t vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GL_FLOAT), &data, GL_STATIC_DRAW);
		glVertexAttribPointer(attribNr, 3, GL_FLOAT, false, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void unbindVAO()
	{
		glBindVertexArray(0);
	}

	uint32_t loadTexture(const char* filename, std::string &directory)
	{
		std::string fullpath = std::string(filename);
		fullpath = directory + '/' + filename;

		loadImage(fullpath.c_str());

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

private:
	std::list<int> vaos;
	std::list<int> vbos;
	std::list<int> textures;

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	uint32_t texID;
	unsigned char* m_TextureData;
	int m_Width, m_Height;

	void loadImage(const char* filename)
	{
		FILE *file = 0;
		file = std::fopen(filename, "rb");
		if (!file)
			std::cout << "File not found" << std::endl;
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, file);
		if (bfh.bfType != 0x4D42)
			std::cout << "Not a valid file" << std::endl;
		fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);
		if (bih.biSizeImage == 0)
			bih.biSizeImage = bih.biHeight * bih.biWidth * 3;
		m_TextureData = new unsigned char[bih.biSizeImage];
		fseek(file, bfh.bfOffBits, SEEK_SET);
		fread(m_TextureData, 1, bih.biSizeImage, file);
		unsigned char temp;
		for (int i = 0; i < bih.biSizeImage; i += 3)
		{
			temp = m_TextureData[i];
			m_TextureData[i] = m_TextureData[i + 2];
			m_TextureData[i + 2] = temp;
		}
		m_Width = bih.biWidth;
		m_Height = bih.biHeight;
		fclose(file);
	}
};

class GuiRenderer
{
public:
	GuiRenderer(Loader loader)
	{
		std::vector<float> positions = { -1, 1, -1, -1, 1, 1, 1, -1 };
		m_Quad = loader.loadToVAO(positions);
	}

	void render(std::vector<GUITex> guis, Shader &shader, Model &model, glm::mat4 matrix, glm::vec2 pos)
	{
		shader.use();
		glBindVertexArray(m_Quad.getVaoID());
		glEnableVertexAttribArray(0);
		glDisable(GL_DEPTH_TEST);
		for (GUITex gui : guis)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gui.GetTex());
			matrix = createTransformMat(gui.GetPos(), gui.GetScale());
			shader.setMat4("transformationMatrix", matrix);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Quad.getVertexCount());
		}
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		renderModel(model, shader, matrix);
	}

	glm::mat4 createTransformMat(glm::vec2 translation, glm::vec2 scale)
	{
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(translation.x, translation.y, 0.0));
		mat = glm::scale(mat, glm::vec3(scale.x, scale.y, 1.0f));
		return mat;
	}

private:
	QuadModel m_Quad;
};

