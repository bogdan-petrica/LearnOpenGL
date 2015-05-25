#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <vector>

class Texture
{
public:
    // Constructor generates the shader on the fly
    Texture();
    ~Texture();

    bool
    loadFromFile(const char* path, bool haveAlpha = false);

    void
    upload(unsigned char* image, int w, int h, bool hasTransparency = false);

    void
    bind();

    void
    unbind();

private:

    std::vector<GLuint> mTextures;
};

#endif // TEXTURE_H