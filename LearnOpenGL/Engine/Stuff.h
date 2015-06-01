#pragma once
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL.h"

class Stuff
{
public:
	static void generateNicePosition(float in, float& outx, float& outy);
};

class Texture
{
public:
	Texture(const std::string& fileName)
	{
		int width, height;
		unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		glGenTextures(1, &mTexture);

		glBindTexture(GL_TEXTURE_2D, mTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GLuint getTextureId() const { return mTexture; }
private:
	GLuint mTexture;
};