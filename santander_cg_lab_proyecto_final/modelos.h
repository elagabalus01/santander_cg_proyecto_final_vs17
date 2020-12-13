#pragma once
#include <iostream>
#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>
int myVar=20;
float vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-6.46f, 4.0f-1.25f, -6.0f-1.25f,    1.0f, 0.0f,0.0f,		0.0f,0.0f,
		-6.46f, 4.0f-1.25f, -6.0f+1.25f,	   1.0f, 0.0f,0.0f,		1.0f,0.0f,
		-6.46f, 4.0f+ 1.25f,-6.0f+1.25f,     1.0f, 0.0f,0.0f,		1.0f,1.0f,
		-6.46f, 4.0f+1.25f, -6.0f-1.25f,    1.0f, 0.0f,0.0f,		0.0f,1.0f
};
int indices[] ={
	0,1,3,
	1,2,3
};


float vertices2[] =
{
	// Positions            // Normals              // Texture Coords
	6.46f, 4.0f - 1.25f, -1.0f - 1.25f,    1.0f, 0.0f,0.0f,		0.0f,0.0f,
	6.46f, 4.0f - 1.25f, -1.0f + 1.25f,	   1.0f, 0.0f,0.0f,		1.0f,0.0f,
	6.46f, 4.0f + 1.25f,-1.0f + 1.25f,     1.0f, 0.0f,0.0f,		1.0f,1.0f,
	6.46f, 4.0f + 1.25f, -1.0f - 1.25f,    1.0f, 0.0f,0.0f,		0.0f,1.0f
};
int indices2[] = {
	0,1,3,
	1,2,3
};
class Poster {
public:
	GLuint VBO, VAO, EBO;
	GLuint texture1;
	float *vertices;
	int* indices;
	int vertices_size = 0, indinces_size = 0;
	char* path;

	Poster(char* path,float *vertices, int *indices,int vertices_size,int indices_size)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->vertices_size = vertices_size;
		this->indinces_size = indices_size;
		this->path = path;
	}
	void makePoster() {

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices_size, this->vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->indinces_size, this->indices, GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Normals attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// Texture Coordinate attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		glGenTextures(1, &this->texture1);

		int textureWidth, textureHeight, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* image;
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		
		// Diffuse map
		image = stbi_load(this->path, &textureWidth, &textureHeight, &nrChannels, 0);
		glBindTexture(GL_TEXTURE_2D, this->texture1);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			exit(-1);
		}
		stbi_image_free(image);
	}

	void drawPoster() {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture1);

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	
		
		
		//glBindTexture(GL_TEXTURE_2D, 0);

	}
};

void printSaludo() {
	std::cout << "Se cargo la librería de modelos.h" << std::endl;
}
