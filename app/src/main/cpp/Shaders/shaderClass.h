#ifndef SHOOTERGAME_SHADER_H
#define SHOOTERGAME_SHADER_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include <android/log.h>
#include <android/asset_manager.h>

class Shader {
    public:
        GLuint ID;
        Shader(const char* vertexFile, const char* fragmentFile, AAssetManager* g_assetManager);

        void Activate();
        void Delete();
};


#endif //SHOOTERGAME_SHADER_H
