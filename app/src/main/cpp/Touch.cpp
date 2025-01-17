#include "Touch.h"

glm::vec2 getMotionXY(android_app *app, bool* touch, glm::vec2& motionXY) {
    android_input_buffer* inputBuffer = android_app_swap_input_buffers(app);

    if (inputBuffer) {
        for (size_t i = 0; i < inputBuffer->motionEventsCount; i++) {
            GameActivityMotionEvent *motionEvent = &inputBuffer->motionEvents[i];

            int action = motionEvent->action;
            int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
            int ptrIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

            float x = GameActivityPointerAxes_getX(&motionEvent->pointers[ptrIndex]);
            float y = GameActivityPointerAxes_getY(&motionEvent->pointers[ptrIndex]);
            motionXY = glm::vec2(x, y);

            if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_MOVE || actionMasked == AMOTION_EVENT_ACTION_UP) {
                (*touch) = true;
                x = GameActivityPointerAxes_getX(&motionEvent->pointers[0]);
                y = GameActivityPointerAxes_getY(&motionEvent->pointers[0]);
                motionXY = glm::vec2(x, y);
            }
        }

        android_app_clear_motion_events(inputBuffer);
    }

    return motionXY;
}

glm::vec3 convertNDC(glm::vec2 motionXY, int width, int height) {
    // Convert screen coordinates to normalized device coordinates (-1 to 1 range)
    float x = (2.0f * motionXY.x) / (float) width - 1.0f;
    float y = 1.0f - (2.0f * motionXY.y) / (float) height;
    float z = 1.0f;

    // Create ray in NDC space (pointing into the screen)
    glm::vec3 ray_ndc = glm::vec3(x, y, z);

    return ray_ndc;
}
