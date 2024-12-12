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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    ptrShader = new Shader("default.vert", "default.frag", g_assetManager);

    ptrVAO_ = new VAO();
    ptrVAO_->bind();

    ptrVBO_ = new VBO();
    ptrVBO_->bind();
    ptrVBO_->addVertices(sizeof(float) * 3 * 8, vertices);

    ptrEBO_ = new EBO(skyboxIndices);

    ptrVAO_->LinkAttrib(0, 3, GL_FLOAT, sizeof(GLfloat) * 3, nullptr);

    ptrVAO_->unbind();
    ptrVBO_->unbind();
    ptrEBO_->unbind();
}

Renderer::~Renderer() { // Dis-construct for when the Function is terminating
    ptrShader->Delete();
    ptrVAO_->Delete();
    ptrVBO_->Delete();
    ptrEBO_->Delete();

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

    ptrShader->Activate();
    ptrVAO_->bind();
    glDrawElements(GL_LINES, skyboxIndices.size() * sizeof(int), GL_UNSIGNED_INT, 0);
    ptrVAO_->unbind();

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}
