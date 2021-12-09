#ifndef CUBE_H_
#define CUBE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "ray.h"
#include "triangle.h"

using std::vector;
using std::pair;

enum Face {FACE_BACK, FACE_FRONT, FACE_LEFT, FACE_RIGHT, FACE_BUTTOM, FACE_TOP};
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
            face_textures[FACE_BACK] = back;
            face_textures[FACE_FRONT] = front;
            face_textures[FACE_LEFT] = left;
            face_textures[FACE_RIGHT] = right;
            face_textures[FACE_BUTTOM] = bottom;
            face_textures[FACE_TOP] = top;
    }

    /*
     * Set texture for each face of the cube.
     * This function has the same signature as the second constructor.
     * 
     * @param back: texture index for back face
     */
    void setFaceTexture(Face f, FaceTexture t){
        face_textures[f] = t;
    }

    void setModel(const glm::mat4& model_){
        model = model_;
    }

    glm::vec3 getCenter() const {
        return glm::vec3(model * glm::vec4(0, 0, 0, 1.0f));
    }

    void rotate(glm::vec3 axis, glm::vec3 center, const float angle){
        glm::mat4 tmp_model;
        tmp_model = glm::translate(glm::mat4(1.0f), center);
        tmp_model = glm::rotate(tmp_model, glm::radians(angle), axis);
        tmp_model = glm::translate(tmp_model, -center);

        model = tmp_model * model;
    }
    
    /*
     * Draw current cube.
     * First initialize VAO & VBO if the first time drawing.
     * 
     * @param textures: textures handlers intialized and loaded.
     */
    void draw(const Shader& shader, GLuint* textures, glm::vec3 axis, glm::vec3 center, const float angle){
        if(first_draw) initDrawing();
        glBindVertexArray(VAO);

        FaceTexture currTex;
        glm::mat4 tmp_model;
        glm::mat3 normModel;
        tmp_model = glm::translate(glm::mat4(1.0f), center);
        tmp_model = glm::rotate(tmp_model, glm::radians(angle), axis);
        tmp_model = glm::translate(tmp_model, -center);
        normModel = glm::mat3(glm::transpose(glm::inverse(tmp_model * model)));

        for(int ix = 0; ix != 6; ++ix){
            currTex = face_textures[ix];
			glBindTexture(GL_TEXTURE_2D, textures[currTex]);
            shader.setMat4("model", tmp_model * model);
            shader.setMat3("normModel", normModel);
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
        glm::vec3 hit_point, norm;

        for(int ix = 0; ix != 6; ++ix){
            Triangle tri(vertices[6*ix].first, 
                         vertices[6*ix+1].first, 
                         vertices[6*ix+2].first);
            tri.transform(model);
            norm = glm::normalize(glm::cross(tri.y-tri.x, tri.z-tri.x));
            dn = glm::dot(ray.direction, norm);
            if(fabs(dn) < 1e-5) continue;
            t = glm::dot((tri.x - ray.origin), norm) / dn;
            if(t < t_min || t_max < t) continue;
            hit_point = ray.at(t);
            if(!tri.inside(hit_point)){
                tri = Triangle(vertices[6*ix+3].first, vertices[6*ix+4].first, vertices[6*ix+5].first);
                tri.transform(model);
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
	     /* vertices              texture */
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
    GLfloat norms[108] = {
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 
		0.0f,  0.0f, -1.0f, 

		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
	 	0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
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
        glBufferData(GL_ARRAY_BUFFER, 36*5*sizeof(GLfloat)+sizeof(norms), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 36*5*sizeof(GLfloat), &vertices[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 36*5*sizeof(GLfloat), sizeof(norms), norms);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)(36*5*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        first_draw = false;
    }
};

#endif
