#ifndef SHOOTERGAME_RENDERER_H
#define SHOOTERGAME_RENDERER_H

#include <GLES/egl.h> // egl servers to display what we are going to render just like glfw
#include <GLES3/gl3.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "Loader/Loader.h"

class Renderer {
    public:
        Renderer(android_app *app, AAssetManager* g_assetManager);
        ~Renderer();

        void do_frame();

    private:
        EGLDisplay display; // Serves as connection for both Android Display and GLES
        EGLConfig config;
        EGLSurface surface;
        EGLContext context; // to run OpenGL calls

        Loader* ptrLoader;
};


#endif //SHOOTERGAME_RENDERER_H
