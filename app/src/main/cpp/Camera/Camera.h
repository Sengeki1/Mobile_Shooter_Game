#ifndef SHOOTERGAME_CAMERA_H
#define SHOOTERGAME_CAMERA_H

#include <android/input.h>
#include <GLES3/gl3.h>
#include "../Libraries/glm/glm.hpp"
#include "../Libraries/glm/gtc/matrix_transform.hpp"
#include "../Libraries/glm/gtc/type_ptr.hpp"
#include "../Libraries/glm/gtx/rotate_vector.hpp"
#include "../Libraries/glm/gtx/vector_angle.hpp"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "../Shaders/shaderClass.h"

class Camera {

    public:
        void setCamera(int width, int height, Shader &shader, glm::mat4 (*projPtr) (int width, int height, Shader &shader));
        void mouse(double xpos, double ypos);
        bool firstTouch = true;
        glm::vec3 position = glm::vec3(0.0f);

    private:
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
        float width, height;
        double yaw, pitch;
        float lastX, lastY;
        float sensivity = 0.1f;
};


#endif //SHOOTERGAME_CAMERA_H
