#include "Renderer.h"

Renderer::Renderer(android_app *app, AAssetManager* g_assetManager) { // Construct

    // SCREEN CONFIGURATIONS
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY); // set the Display as default
    assert(display); // In case of errors;

    auto res = eglInitialize(display, nullptr, nullptr); // auto serves as an autocomplete to guess the type of the result
    assert(res == EGL_TRUE);

    { // Define a scope
        EGLint attribs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RED_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_NONE
        };

        EGLint num_configs; // used to check the return if 0 it didn't find any configuration that matches the attributes that we give it
        eglChooseConfig(display, attribs, &config, 1, &num_configs);
        assert(num_configs == 1);
    }

    surface = eglCreateWindowSurface(display, config, app->window, nullptr); // We dont pass the attrib list since we dont care about the window itself what we are going to render is in the EGL
    assert(surface != EGL_NO_SURFACE);

    {
        EGLint attribs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE
        };
        context = eglCreateContext(display, config, nullptr, attribs);
        assert(context != EGL_NO_CONTEXT);
    }

    // Activate Current Context
    res = eglMakeCurrent(display, surface, surface, context);
    assert(res);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    ptrLoader = new Loader(g_assetManager);
}

Renderer::~Renderer() { // Dis-construct for when the Function is terminating
    ptrLoader->DeleteMeshes();

    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

void Renderer::do_frame(glm::vec2 motionXY, bool* touch) {
    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH,
                    &width); // get width and height of the phone screen
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // deltaTime
    static float angle = 0.0f; // space for the static variable is allocated only once and the value of the variable in the previous call gets carried through the next function call.
    angle += 1.0f;

    ptrLoader->RenderMeshes(width, height, angle, motionXY, &(*touch));

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}
