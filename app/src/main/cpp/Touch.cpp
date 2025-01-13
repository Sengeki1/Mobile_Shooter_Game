#include "Touch.h"

glm::vec2 getMotionXY(android_app *app, bool* touch) {
    android_input_buffer* inputBuffer = android_app_swap_input_buffers(app);
    glm::vec2 motionXY = glm::vec2(-1, -1);

    if (inputBuffer) {
        for (size_t i = 0; i < inputBuffer->motionEventsCount; i++) {
            GameActivityMotionEvent *motionEvent = &inputBuffer->motionEvents[i];

            int action = motionEvent->action;
            int actionMasked = action & AMOTION_EVENT_ACTION_MASK;

            if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_MOVE || actionMasked == AMOTION_EVENT_ACTION_UP) {
                float x = GameActivityPointerAxes_getX(&motionEvent->pointers[0]);
                float y = GameActivityPointerAxes_getY(&motionEvent->pointers[0]);

                motionXY = glm::vec2(x, y);
                (*touch) = true;
            }
        }

        android_app_clear_motion_events(inputBuffer);
    }

    return motionXY;
}
