#ifndef SHOOTERGAME_TOUCH_H
#define SHOOTERGAME_TOUCH_H

#include <android/input.h>
#include <GLES3/gl3.h>
#include <vector>
#include "../Mesh/Square.h"
#include "Libraries/glm/glm.hpp"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <android/log.h>

 glm::vec2 getMotionXY(android_app *app, bool* touch, bool* button_touch, glm::vec2& motionXY);

 glm::vec3 convertNDC(glm::vec2 motionXY, int width, int height);

 std::vector<glm::vec2> min_maxSquare(glm::mat4 &orthographicProjection, float offset_x, float offset_y);

#endif //SHOOTERGAME_TOUCH_H
