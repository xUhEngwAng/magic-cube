#ifndef RAY_H_
#define RAY_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ray{
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray() = default;
    Ray(const glm::vec3& ori, const glm::vec3& dir):
        origin(ori), direction(dir) {}

    glm::vec3 at(float t) const {
        return origin + glm::vec3(t) * direction;
    }
};

#endif

