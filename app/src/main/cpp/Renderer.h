#ifndef SHOOTERGAME_RENDERER_H
#define SHOOTERGAME_RENDERER_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/gtx/string_cast.hpp"

#include "Buffers/VAO.h"
#include "Buffers/VBO.h"
#include "Buffers/EBO.h"
#include "Shaders/shaderClass.h"
#include "Mesh/Cube.h"
#include "Textures/Texture.h"
#include "Loader/Loader.h"

class Renderer {
    public:
        Renderer(android_app *app, AAssetManager* g_assetManager);
        ~Renderer();

        void do_frame();
        void setProjection(Shader *shader, int width, int height);
    private:
        EGLDisplay display; // Serves as connection for both Android Display and GLES
        EGLConfig config;
        EGLSurface surface;
        EGLContext context; // to run OpenGL calls

        VAO* ptrVAO_2;
        VBO* ptrVBO_2;
        EBO* ptrEBO_2;
        Shader* ptrShader_2;
        Texture* ptrTexture;
        Loader* ptrLoader;
};


#endif //SHOOTERGAME_RENDERER_H
