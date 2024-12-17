#include "Renderer.h"

#include <assert.h>

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
    ptrLoader->Mesh();
}

Renderer::~Renderer() { // Dis-construct for when the Function is terminating
    ptrLoader->DeleteMeshes();

    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

void Renderer::do_frame() {
    int width, height;
    eglQuerySurface(display, surface, EGL_WIDTH,
                    &width); // get width and height of the phone screen
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // deltaTime
    static float angle = 0.0f; // space for the static variable is allocated only once and the value of the variable in the previous call gets carried through the next function call.
    angle += 1.0f;

    ptrLoader->RenderMeshes(width, height, angle);

    //ptrShader_2->Activate();
    //Renderer::setProjection(ptrShader_2, width, height);
    //ptrVAO_2->bind();

    // transformations
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f, -0.9f, -3.0f));
    // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(angle * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    //glUniformMatrix4fv(glGetUniformLocation(ptrShader_2->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    //glUniform1f(glGetUniformLocation(ptrShader_2->ID, "scale"), 0.5f);

    //glDrawElements(GL_LINE_LOOP, ptrLoader->indices.size(), GL_UNSIGNED_INT, 0);

    auto res = eglSwapBuffers(display, surface);
    assert(res);
}
