#include "Mesh.h"

//--------------------------------------------------------------------Mesh.h---------------------------------------------------------------------------------
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	/* setting the vertex buffers and its attribute pointers. */
	this->setupMesh();
}

void Mesh::draw(Shader shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		/* Active a texture unit before binding */
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream sStream;
		std::string number;
		std::string name = this->textures[i].type;
		if (name == "texture_diffuse")
			sStream << diffuseNr++;
		else if (name == "texture_specular")
			sStream << specularNr++;
		else if (name == "texture_normal")
			sStream << normalNr++;
		else if (name == "texture_height")
			sStream << heightNr++;

		number = sStream.str();

		glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		/* bind the texture */
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	/* set each mesh's shininess property to a default value */
	//glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 10.0f);

	/** Draw mesh */
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	/* setting all to default */
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setupMesh()
{

	/* Create buffers/arrays */
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	/* Load data into vertex buffers */
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
	//vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
	//vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
