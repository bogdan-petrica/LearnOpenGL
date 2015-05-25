#include "texture.h"

#include <SOIL.h>

/*#include <string>
#include <fstream>
#include <sstream>
#include <iostream>*/

Texture::Texture()
{
}

Texture::~Texture()
{
    for (int i = 0; i < mTextures.size(); ++i)
    {
        glDeleteTextures(1, &mTextures[i]);
    }
};

bool
Texture::loadFromFile(const char* path, bool haveAlpha/*=false*/)
{
    int w, h;
    unsigned char* image = SOIL_load_image(path, &w, &h, 0,
        haveAlpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    if(image != nullptr)
    {
        upload(image, w, h, haveAlpha);
        SOIL_free_image_data(image);
        return true;
    }
    return false;
}

void
Texture::upload(unsigned char* data, int w, int h, bool hasTransparency/*=false*/)
{
    mTextures.push_back(-1);
    glGenTextures(1, &(*(mTextures.end() - 1)));
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, hasTransparency ? GL_RGBA : GL_RGB,
        w, h, 0, hasTransparency ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}
// Uses the current shader
void
Texture::bind()
{
    for (int i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
    }
}

void
Texture::unbind()
{
    for (int i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}