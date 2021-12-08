#ifndef TRIANGLE_H_
#define TRIANGLE_H_

struct HitRecord{
    float t;
    glm::vec3 p;
};

class Triangle{
public:
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;

    Triangle() = default;
    Triangle(const glm::vec3& x_, const glm::vec3& y_, const glm::vec3& z_):
        x(x_), y(y_), z(z_) {}

    bool inside(const glm::vec3& p){
        glm::vec3 v1 = glm::cross(x-p, y-p);
        glm::vec3 v2 = glm::cross(y-p, z-p);
        glm::vec3 v3 = glm::cross(z-p, x-p);
        float s1 = glm::dot(v1, v2);
        float s2 = glm::dot(v1, v3);
        float s3 = glm::dot(v2, v3);
        
        bool has_neg = (s1 < 0 || s2 < 0 || s3 < 0);
        bool has_pos = (s1 > 0 || s2 > 0 || s3 > 0);
        return !(has_neg && has_pos);
    }
};

#endif

