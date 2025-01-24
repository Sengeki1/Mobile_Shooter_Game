#ifndef SHOOTERGAME_RENDERER_H
#define SHOOTERGAME_RENDERER_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <assert.h>

#include "Loader/Loader.h"
#include "../Camera/Camera.h"

class Renderer {
    public:
        Renderer(android_app *app, AAssetManager* g_assetManager);
        ~Renderer();

        void do_frame(glm::vec2 motionXY, bool* touch, bool* button_touch, android_app *app);

        Loader* ptrLoader;
        glm::vec2 motionXY = glm::vec2(-1, -1);

    private:
        EGLDisplay display; // Serves as connection for both Android Display and GLES
        EGLConfig config;
        EGLSurface surface;
        EGLContext context; // to run OpenGL calls
};


#endif //SHOOTERGAME_RENDERER_H
