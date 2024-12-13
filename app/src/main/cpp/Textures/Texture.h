#ifndef SHOOTERGAME_TEXTURE_H
#define SHOOTERGAME_TEXTURE_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

#include <android/log.h>
#include <android/asset_manager.h>

#include "../Libraries/stb/stb_image.h"

class Texture {
    public:
        Texture(const char* filename, AAssetManager* g_assetManager);
        ~Texture();

        GLuint texture;
private:
    int textureWidth, textureHeight;
        unsigned char* bytes;
};


#endif //SHOOTERGAME_TEXTURE_H
