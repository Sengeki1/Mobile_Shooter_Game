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

    GLfloat vertices[] = {
                -0.4f, -0.4f,  0.4f,    //        7--------6
                 0.4f, -0.4f,  0.4f,    //       /|       /|
                 0.4f, -0.4f, -0.4f,    //      4--------5 |
                 -0.4f, -0.4f, -0.4f,   //      | |      | |
                 -0.4f,  0.4f,  0.4f,   //      | 3------|-2
                 0.4f,  0.4f,  0.4f,    //      |/       |/
                 0.4f,  0.4f, -0.4f,    //      0--------1
                 -0.4f,  0.4f, -0.4f
    };

    unsigned int skyboxIndices[] =
            {
                    // Right
                    1, 2, 6,
                    6, 5, 1,
                    // Left
                    0, 4, 7,
                    7, 3, 0,
                    // Top
                    4, 5, 6,
                    6, 7, 4,
                    // Bottom
                    0, 3, 2,
                    2, 1, 0,
                    // Back
                    0, 1, 5,
                    5, 4, 0,
                    // Front
                    3, 7, 6,
                    6, 2, 3
            };

    ptrShader = new Shader("default.vert", "default.frag", g_assetManager);

    ptrVAO_ = new VAO();
    ptrVAO_->bind();

    ptrVBO_ = new VBO();
    ptrVBO_->bind();
    ptrVBO_->addVertices(sizeof(float) * 3 * 8, vertices);

    ptrVAO_->LinkAttrib(0, 3, GL_FLOAT, sizeof(GLfloat) * 3, nullptr);

    ptrVAO_->unbind();
    ptrVBO_->unbind();
}

Renderer::~Renderer() { // Dis-construct for when the Function is terminating
    ptrShader->Delete();
    ptrVAO_->Delete();
    ptrVBO_->Delete();

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
    glDrawArrays(GL_TRIANGLES, 0, 36);
    ptrVAO_->unbind();

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}
