#include "Touch.h"

glm::vec2 getMotionXY(android_app *app, bool* touch, bool* button_touch, glm::vec2& motionXY) {
    android_input_buffer* inputBuffer = android_app_swap_input_buffers(app);

    if (inputBuffer) {
        for (size_t i = 0; i < inputBuffer->motionEventsCount; i++) {
            GameActivityMotionEvent *motionEvent = &inputBuffer->motionEvents[i];

            int action = motionEvent->action;
            int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
            int ptrIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

            float x = GameActivityPointerAxes_getX(&motionEvent->pointers[ptrIndex]);
            float y = GameActivityPointerAxes_getY(&motionEvent->pointers[ptrIndex]);
            (*button_touch) = true;
            motionXY = glm::vec2(x, y);

            if (actionMasked == AMOTION_EVENT_ACTION_DOWN || actionMasked == AMOTION_EVENT_ACTION_MOVE) {
                (*touch) = true;
                x = GameActivityPointerAxes_getX(&motionEvent->pointers[0]);
                y = GameActivityPointerAxes_getY(&motionEvent->pointers[0]);
                motionXY = glm::vec2(x, y);
            }

            if (actionMasked == AMOTION_EVENT_ACTION_UP) {
                (*button_touch) = false;
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

std::vector<glm::vec2> min_maxSquare(glm::mat4 &orthographicProjection, float offset_x, float offset_y) {
    std::vector<glm::vec3> transformed_vertices;

    for (int j = 0; j < square_vertices.size(); j++) {
        transformed_vertices.emplace_back(orthographicProjection * glm::vec4((square_vertices[j].x + offset_x) * 0.25f,
                                                                             (square_vertices[j].y + offset_y) * 0.25f, 0.0f, 1.0f));
    }

    auto max = glm::vec2(transformed_vertices[0]);
    auto min = glm::vec2(transformed_vertices[0]);

    for (auto& vertex : transformed_vertices) {
        max = glm::max(glm::vec2(vertex), max);
        min = glm::min(glm::vec2(vertex), min);
    }

    return { min, max };
}
