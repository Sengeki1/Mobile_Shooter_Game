#ifndef SHOOTERGAME_TEXTURE_H
#define SHOOTERGAME_TEXTURE_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

#include <android/log.h>
#include <android/asset_manager.h>
#include <string>
#include "../Buffers/VAO.h"
#include "../Buffers/VBO.h"
#include "../Buffers/EBO.h"
#include "../Shaders/shaderClass.h"
#include "../Mesh/Cube.h"

#include "../Libraries/stb/stb_image.h"

static std::string facesCubeMap[6] = {
        "Skybox/px.bmp",
        "Skybox/nx.bmp",
        "Skybox/py.bmp",
        "Skybox/ny.bmp",
        "Skybox/pz.bmp",
        "Skybox/nz.bmp"
};

class Texture {
    public:
        //Texture(const char* filename, AAssetManager* g_assetManager);
        Texture(AAssetManager* g_assetManager, VAO* VAO, VBO* VBO, EBO* EBO);
        ~Texture();

        GLuint texture;
        GLuint cubeMapTexture;
    private:
};


#endif //SHOOTERGAME_TEXTURE_H
