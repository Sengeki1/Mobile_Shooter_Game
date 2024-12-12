#include "Renderer.h"

#include <assert.h>

Renderer::Renderer(android_app *app, AAssetManager* g_assetManager) { // Construct
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

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    float vertices[] = {
             0.0f, 0.5f,
            -0.5f, -0.5f,
             0.5f, -0.5f
    };

    Shader shaderProgram("default.vert", "default.frag", g_assetManager);
    ptrShader = &shaderProgram;

    VAO VAO_;
    VAO_.bind();
    ptrVAO_ = &VAO_;

    VBO VBO_;
    VBO_.bind();
    ptrVBO_ = &VBO_;
    VBO_.addVertices(sizeof(vertices), vertices);

    VAO_.LinkAttrib(0, 2, GL_FLOAT, sizeof(float) * 2, (void*)0);

    VAO_.unbind();
    VBO_.unbind();
}

Renderer::~Renderer() { // Dis-construct for when the Function is terminating
    (*ptrVAO_).Delete();
    (*ptrVBO_).Delete();
    (*ptrShader).Delete();

    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

void Renderer::do_frame() {
    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH, &width); // get width and height of the phone screen
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 0.5f, 1.0f, 1.0f);

    (*ptrShader).Activate();
    (*ptrVAO_).bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    (*ptrVAO_).unbind();

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}
