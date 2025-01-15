#ifndef SHOOTERGAME_TOUCH_H
#define SHOOTERGAME_TOUCH_H

#include <android/input.h>
#include <GLES3/gl3.h>
#include "Libraries/glm/glm.hpp"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <android/log.h>

 glm::vec2 getMotionXY(android_app *app, bool* touch, glm::vec2& motionXY);

 glm::vec3 convertNDC(glm::vec2 motionXY, int width, int height, glm::mat4 projection);

#endif //SHOOTERGAME_TOUCH_H
