#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

#include <android/log.h>
#include "Renderer.h"
#include "AssetManager.h"

AAssetManager *g_assetManager = nullptr; // asset manager for loading files set a value to it for init

extern "C" {
    #include <game-activity/native_app_glue/android_native_app_glue.c>

    void on_app_cmd(android_app *app, int32_t cmd);
    void android_main(android_app *app);
}

void on_app_cmd(android_app *app, int32_t cmd) { // required callback function to process functionalities on your app
    switch(cmd) {
        case APP_CMD_INIT_WINDOW: // starting app
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Starting Window.."); // print message when starting app
            g_assetManager = app->activity->assetManager;  // when app loading set the assetManager
            app->userData = new Renderer(app, g_assetManager);
            break;
        case APP_CMD_TERM_WINDOW: {
            /* Without brackets, you can't declare new variables in a case statement due to C++ syntax rules.
             * The brackets allow you to declare local variables within that specific case.*/
            __android_log_print(ANDROID_LOG_INFO, "LOG", "Terminating Window..");
            Renderer *renderer = (Renderer *) app->userData;
            /* So what is happening is that we create a Pointer to Renderer that will store the address
             * that points to the object userData which contains the Renderer Object, for that we create
             * a type casting and using delete we delete the object at that address*/
            if (renderer) {
                delete renderer;
                app->userData = nullptr;
            }
            break;
        }
        default:
            break;
    }
}

void android_main(android_app *app) {

    app->onAppCmd = on_app_cmd; // callBack required

    android_poll_source *poll_source; // It is a struct that represents a source of input or events in Android's native event system.
    int events;

    do {
        // Handle Events
        if ((ALooper_pollAll(0, nullptr, &events, (void **) &poll_source)) >= 0) { // Alooper_pollAll It waits for events to occur or retrieves pending events if available, processing them one at a time.
            if (poll_source) poll_source->process(app, poll_source);
        }

        if (!app->userData) continue; // if the userData returns null

        Renderer* renderer = (Renderer*) app->userData;
        renderer->do_frame();

    } while (!app->destroyRequested);
}