#ifndef SHOOTERGAME_TOUCH_H
#define SHOOTERGAME_TOUCH_H

#include <android/input.h>
#include <GLES3/gl3.h>
#include "Libraries/glm/vec2.hpp"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <android/log.h>

 glm::vec2 getMotionXY(android_app *app, bool* touch);

#endif //SHOOTERGAME_TOUCH_H
