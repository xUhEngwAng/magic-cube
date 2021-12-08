#ifndef CUBE_H_
#define CUBE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "ray.h"
#include "triangle.h"

using std::vector;
using std::pair;

enum FaceTexture { FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_2, FACE_TEXTURE_3, FACE_TEXTURE_4, FACE_TEXTURE_5, FACE_TEXTURE_6 };

class Cube {
public:
    /*
     * Default Constructor. 
     * Default to blank texture(FACE_TEXTURE_0) for each face.
     */
    Cube() {
        for(int ix = 0; ix != 6; ++ix){
            face_textures[ix] = FACE_TEXTURE_0;
        }
    }

    /*
     * A more dedicated constructor.
     * Explicitly set texture for interested face.
     * 
     * @param back: texture index for back face
     * 
     */
    Cube(FaceTexture back, FaceTexture front,
         FaceTexture left, FaceTexture right,
         FaceTexture bottom, FaceTexture top){
            face_textures[0] = back;
            face_textures[1] = front;
            face_textures[2] = left;
            face_textures[3] = right;
            face_textures[4] = bottom;
            face_textures[5] = top;
    }

    /*
     * Set texture for each face of the cube.
     * This function has the same signature as the second constructor.
     * 
     * @param back: texture index for back face
     */
    void setFaceTexture(FaceTexture back, FaceTexture front,
                        FaceTexture left, FaceTexture right,
                        FaceTexture bottom, FaceTexture top){
            face_textures[0] = back;
            face_textures[1] = front;
            face_textures[2] = left;
            face_textures[3] = right;
            face_textures[4] = bottom;
            face_textures[5] = top;
    }

    void setModel(const glm::mat4& model_){
        model = model_;
    }

    void rotate(glm::vec3 axis, const float angle){
        axis = glm::normalize(axis);
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * model;
    }
    
    /*
     * Draw current cube.
     * First initialize VAO & VBO if the first time drawing.
     * 
     * @param textures: textures handlers intialized and loaded.
     */
    void draw(const Shader& shader, GLuint* textures, const glm::mat4& tmp_model){
        if(first_draw) initDrawing();
        glBindVertexArray(VAO);

        FaceTexture currTex;

        for(int ix = 0; ix != 6; ++ix){
            currTex = face_textures[ix];
			glBindTexture(GL_TEXTURE_2D, textures[currTex]);
            shader.setMat4("model", tmp_model * model);
			glDrawArrays(GL_TRIANGLES, ix * 6, 6);
		}

        // Unbind VAO for further drawing
        glBindVertexArray(0);
    }

    /*
     * Release VAO & VBO after drawing completed
     */
    void finishDrawing(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec){
        bool ishit = false;
        float dn, t;
        glm::vec3 hit_point;

        for(int ix = 0; ix != 6; ++ix){
            Triangle tri(vertices[6*ix].first, vertices[6*ix+1].first, vertices[6*ix+2].first);
            glm::vec3 norm = glm::cross(tri.y-tri.x, tri.z-tri.x);
            dn = glm::dot(ray.direction, norm);
            if(fabs(dn) < 1e-5) continue;
            t = glm::dot((tri.x - ray.origin), norm) / dn;
            if(t < t_min || t_max < t) continue;
            hit_point = ray.at(t);
            if(!tri.inside(hit_point)){
                tri = Triangle(vertices[6*ix+3].first, vertices[6*ix+4].first, vertices[6*ix+5].first);
                if(!tri.inside(hit_point)) continue;
            }
            ishit = true;
            t_max = t;
            rec.t = t;
            rec.p = hit_point;
        }

        return ishit;
    }

private:
    vector<pair<glm::vec3, glm::vec2>> vertices = {
	     /* vertices          texture */
	  	 // Back face
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},

        // Front face
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

		 // Left face
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

		 // Right face
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

		 // Bottom face
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

		 // Top face
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}}
    };
    FaceTexture face_textures[6];
    GLuint VAO, VBO;
    glm::mat4 model;
    bool first_draw = true;

    /*
     * initialize VAO & VBO when drawing for the first time
     */
    void initDrawing(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*5*sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        first_draw = false;
    }
};

#endif
