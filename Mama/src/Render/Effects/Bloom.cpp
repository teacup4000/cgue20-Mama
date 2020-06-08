#include "stdafx.h"
#include "Bloom.h"

void Bloom::GenerateBloomParams(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	glGenFramebuffers(1, &m_BloomFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFbo);
	glGenTextures(2, m_Colorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_Colorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Colorbuffers[i], 0
		);
	}

	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &m_RboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	uint32_t attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	glGenFramebuffers(2, m_PingPongFbo);
	glGenTextures(2, m_PingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFbo[i]);
		glBindTexture(GL_TEXTURE_2D, m_PingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}

void Bloom::Bind()	
{
	//glViewport(0.0f, 0.0f, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Bloom::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::RenderBlur(Shader &shader, bool &horizontal, bool &firstIteration)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFbo[horizontal]);
	shader.setInt("horizontal", horizontal);
	glBindTexture(GL_TEXTURE_2D, firstIteration ? m_Colorbuffers[1] : m_PingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
}
void Bloom::RenderBloom(Shader &shader, bool &horizontal)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Colorbuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_PingpongColorbuffers[!horizontal]);
	shader.setFloat("exposure", 1.0f);
}

void Bloom::Postprocess(Shader &shader1, Shader &shader2)
{
	bool horizontal = true, firstIt = true;
	GLuint amount = 10;
	shader1.use();
	for (uint32_t j = 0; j < amount; j++) {
		//renderBlur(blur, horizontal, firstIt);
		//RenderBlur(shader1, horizontal, firstIt);
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFbo[horizontal]);
		shader1.setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, firstIt ? m_Colorbuffers[1] : m_PingpongColorbuffers[!horizontal]);

		RenderQuad();
		horizontal = !horizontal;
		if (firstIt)
			firstIt = false;
	}

	Unbind();
	//RenderBloom(shader2, horizontal);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader2.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Colorbuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_PingpongColorbuffers[!horizontal]);
	shader2.setFloat("exposure", 1.0f);
	RenderQuad();
}

void Bloom::Destroy()
{
	glDeleteFramebuffers(1, &m_BloomFbo);
	glDeleteFramebuffers(1, &m_RboDepth);
	glDeleteFramebuffers(2, m_PingPongFbo);
}


void Bloom::RenderQuad()
{
	if (m_QuadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &m_QuadVAO);
		glGenBuffers(1, &m_QuadVBO);
		glBindVertexArray(m_QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}