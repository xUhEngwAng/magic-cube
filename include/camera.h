#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
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

    Camera(glm::vec3 pos, float aspect_ratio): 
        position(pos), aspect_ratio(aspect_ratio), target(glm::vec3(0)) { 
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

    glm::mat4 getPerspective() {
        return glm::perspective(glm::radians(fov), aspect_ratio, znear, zfar);
    }

    glm::vec3 at(float u, float v){
        return lower_left_corner + u * horizontal + v * vertical;
    }

    // get Field of View
    GLfloat getFoV() const {
        return fov;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    void setAspectRatio(float aspect_ratio_) {
        aspect_ratio = aspect_ratio_;
        updateCameraCoordinates();
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

    void onZooming(float offset) {
        fov -= offset;
        if(fov > 75.0f) fov = 75.0f;
        if(fov < 10.0f)  fov = 10.0f;
        updateCameraCoordinates();
    }

private:
    // perspective settings
    float fov = 45.0f;
    float znear = 0.1f;
    float zfar = 100.0f;
    float aspect_ratio;
    // camera attributes
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0, 1.0f, 0);
    glm::vec3 up;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 lower_left_corner;

    void updateCameraCoordinates(){
        glm::vec3 direction;
        direction = target - position;
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(worldUp, front));
        up    = glm::normalize(glm::cross(front, right));

        float camera_height = 2.0 * znear * std::tan(glm::radians(fov / 2));
        float camera_width = camera_height * aspect_ratio;
        horizontal = glm::vec3(-camera_width) * right;
        vertical = glm::vec3(camera_height) * up;
        lower_left_corner = position + glm::vec3(znear)*front - glm::vec3(0.5f)*horizontal - glm::vec3(0.5f)*vertical;
    }
};
#endif
