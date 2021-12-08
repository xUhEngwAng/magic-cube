#ifndef RAY_H_
#define RAY_H_

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

