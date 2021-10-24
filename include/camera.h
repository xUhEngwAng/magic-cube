#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tools.hpp"

// Direction enum
enum Direction {FORWARD, BACKWARD, LEFT, RIGHT};

class Camera{
public:
    // Constructors
    // ------------
    Camera(): position(glm::vec3(0, 0, 3.0f)) { updateCameraCoordinates(); }

    Camera(glm::vec3 pos): position(pos) { updateCameraCoordinates(); }

    Camera(float x_pos, float y_pos, float z_pos){
        position.x = x_pos;
        position.y = y_pos;
        position.z = z_pos;

        updateCameraCoordinates();
    }

    // getters
    // -------
    // get View Matrix
    glm::mat4 getView() {
        // return glm::lookAt(position, front + position, up);
        // exercise: implement glm::lookAt manually
        // ----------------------------------------
        glm::mat4 translation(1.0f), rotation;
        translation = glm::translate(translation, -position);
        rotation = glm::mat4(
            glm::vec4(-right, 0),
            glm::vec4(up, 0),
            glm::vec4(-front, 0), // reverse direction of the camera
            glm::vec4(0, 0, 0, 1.0f)
        );
        glm::mat4 ret = glm::transpose(rotation) * translation;
        return ret;
    }
    // get Field of View
    GLfloat getFoV() const {
        return fov;
    }

    // Event Processors
    // ----------------
    void onPositionChange(Direction dir, float offset){
        switch (dir){
        case FORWARD:
            position += offset * front;
            break;
        case BACKWARD:
            position -= offset * front;
            break;
        case LEFT:
            position += offset * right;
            break;
        case RIGHT:
            position -= offset * right;
            break;
        default:
            break;
        }
    }

    void onDirectionChange(float yaw_offset, float pitch_offset) {
        yaw -= yaw_offset;
        pitch -= pitch_offset;
        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;
        updateCameraCoordinates();
    }

    void onZooming(float offset) {
        fov -= offset;
        if(fov > 45.0f) fov = 45.0f;
        if(fov < 1.0f)  fov = 1.0f;
    }

private:
    // euler angles
    float pitch = 0;
    float yaw = 180.0f;
    // field of view
    float fov = 45.0f;
    // camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0, 1.0f, 0);
    glm::vec3 up;

    void updateCameraCoordinates(){
        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(worldUp, front));
        up    = glm::normalize(glm::cross(front, right));
    }
};
#endif
