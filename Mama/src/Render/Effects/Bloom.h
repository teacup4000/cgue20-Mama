#pragma once
#include <cstdint>
#include "../Display/Shader.h"
class Bloom
{
public:
	void GenerateBloomParams(uint32_t width, uint32_t height);
	void Bind();
	void Unbind();
	
	void Destroy();
	void Postprocess(Shader &shader1, Shader &shader2);

private:
	uint32_t m_BloomFbo, m_RboDepth, m_PingPongFbo[2], m_PingpongColorbuffers[2], m_Colorbuffers[2];
	uint32_t m_Width, m_Height;

	uint32_t m_QuadVAO = 0, m_QuadVBO;

	void RenderBlur(Shader &shader, bool &horizontal, bool &firstIteration);
	void RenderBloom(Shader &shader, bool &horizontal);
	void RenderQuad();
};