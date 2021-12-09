#ifndef MAGIC_CUBE_H
#define MAGIC_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "cube.h"

enum RotateState {ROTATE_X, ROTATE_Y, ROTATE_Z, ROTATE_NONE};
enum RotateMode  {ROTATE_GLOBAL, ROTATE_LOCAL};
enum RotateLayer {LAYER_ONE, LAYER_TWO, LAYER_THREE, LAYER_ALL};

class MagicCube {
public:
    /*
     * MagicCube default constructor.
     * Intialize texture image & model matrix for each cube
     */
    MagicCube() {
        /* Initilize face textures of each cube */
        // ---------------------------------------
        // First layer, front row
        cubes[0].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[1].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[2].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_6);
        // First layer, middle row
        cubes[3].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[4].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[5].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_6);
        // First layer, last row
        cubes[6].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[7].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_6);
        cubes[8].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_6);

        // Second layer, front row
        cubes[9].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[10].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[11].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_0);
        // Second layer, middle row
        cubes[12].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[13].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[14].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_0);
        // Second layer, last row
        cubes[15].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[16].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0);
        cubes[17].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_0);

        // Last layer, front row
        cubes[18].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[19].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[20].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_5, FACE_TEXTURE_0);
        // Last layer, middle row
        cubes[21].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[22].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[23].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_5, FACE_TEXTURE_0);
        // Lasty layer, last row
        cubes[24].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[25].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_5, FACE_TEXTURE_0);
        cubes[26].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_5, FACE_TEXTURE_0);

        /* Initialize model matrices for each cube*/
        glm::mat4 model;
        for(int ix = 0; ix != 27; ++ix){
            model = glm::translate(glm::mat4(1.0f), cubePositions[ix]);
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
            cubes[ix].setModel(model);
        }
    }

    /*
    * Load Texture images in batch mode.
    * Note that the images should be in .png format.
    * 
    * @param n: # textures to be loaded
    * @param paths: corresponding texture paths
    * @param textures: pre-allocated array to hold the resulting texture handlers
    */
    void loadTextures(GLuint n, const std::string* paths){
        textures = new GLuint[n];
        for(int ix = 0; ix != n; ++ix){
            glGenTextures(1, textures + ix);
            // glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[ix]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            int width, height, nrchannels;
            unsigned char *data = stbi_load(paths[ix].c_str(), &width, &height, &nrchannels, 0);
            if(!data){
                std::cerr << "Failed to load texture image " << paths[ix] << std::endl;
            }
            else{
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
        }
    }

    /*
     * Draw current magic cube.
     * Call loadTextures before calling this method.
     * 
     * @param shader: shader program used to render the magic cube
     */
    void draw(const Shader& shader, RotateState state, RotateLayer layer, float angle){
        glm::vec3 axis;
        if(state == ROTATE_X) axis = glm::vec3(1.0f, 0, 0);
        if(state == ROTATE_Y) axis = glm::vec3(0, 1.0f, 0);
        if(state == ROTATE_Z) axis = glm::vec3(0, 0, 1.0f);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, axis);

        for(int ix = 0; ix != 27; ++ix){
            //cubes[ix].draw(shader, textures, tmp_model);
            if(cube_qualified(ix, state, layer)){
                cubes[ix].draw(shader, textures, model);
            }
            else{
                cubes[ix].draw(shader, textures, glm::mat4(1.0f));
            }
        }
    }

    bool cube_qualified(int cube_ix, const RotateState state, const RotateLayer layer){
        if(state == ROTATE_NONE) return false;
        if(layer == LAYER_ALL) return true;

        float layer_center;
        if(layer == LAYER_ONE) layer_center = -0.3f;
        else if(layer == LAYER_TWO) layer_center = 0;
        else layer_center = 0.3f;

        bool qualified;
        switch(state){
            case ROTATE_X:
                qualified = fabs(cubes[cube_ix].getCenter().x - layer_center) < 1e-5f;
            break;
            case ROTATE_Y:
                qualified = fabs(cubes[cube_ix].getCenter().y - layer_center) < 1e-5f;
            break;
            case ROTATE_Z:
                qualified = fabs(cubes[cube_ix].getCenter().z - layer_center) < 1e-5f;
            break;
            default:
            break;
        }
        return qualified;
    }

    /*
     * Rotate the magic cube as a whole
     *
     * @param axis: rotation axis
     * @param angle: rotation angle, expressed in degrees
     */
    void rotate(glm::vec3 axis, const float angle){
        for(int ix = 0; ix != 27; ++ix){
            cubes[ix].rotate(axis, angle);
        }
    }

    void rotateX(const glm::vec3& hit_point, const float angle){
        for(int ix = 0; ix != 27; ++ix){
            glm::vec3 tmp = cubes[ix].getCenter();
            if(fabs(tmp.x - hit_point.x) < 0.15)
                cubes[ix].rotate(glm::vec3(1.0f, 0, 0), angle);
        }
    }

    void rotateY(const glm::vec3& hit_point, const float angle){
        for(int ix = 0; ix != 27; ++ix){
            glm::vec3 tmp = cubes[ix].getCenter();
            if(fabs(tmp.y - hit_point.y) < 0.15)
                cubes[ix].rotate(glm::vec3(0, 1.0f, 0), angle);
        }
    }

    void rotateZ(const glm::vec3& hit_point, const float angle){
        for(int ix = 0; ix != 27; ++ix){
            glm::vec3 tmp = cubes[ix].getCenter();
            if(fabs(tmp.z - hit_point.z) < 0.15)
                cubes[ix].rotate(glm::vec3(0, 0, 1.0f), angle);
        }
    }

    /*
     * Rotate the upper layer of the magic cube for a given angle
     *
     * @param angle: rotation angle, expressed in degrees
     */
    // void rotateUpperLayerY(const float angle){
    //     glm::vec3 axis(0, 1.0f, 0);
    //     glm::vec4 tmpPos;
    //     glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    //     for(int ix = 0; ix != 27; ++ix){
    //         tmpPos = model_matrices[ix] * glm::vec4(cubePositions[ix], 1.0f);
    //         if(tmpPos.y < 0.49) continue;
    //         model_matrices[ix] = rotate_matrix * model_matrices[ix];
    //     }
    // }

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec){
        bool ishit = false;
        for(int ix = 0; ix != 27; ++ix){
            if(cubes[ix].hit(ray, t_min, t_max, rec)){
                t_max = rec.t;
                ishit = true;
                rec.ix = ix;
            }
        }
        if(!ishit){
            return false;
        }
        glm::vec3 hit_point = rec.p;
        return true;
    }

private:
    Cube cubes[27];
    glm::vec3 cubePositions[27] = {
        // First Layer
        glm::vec3(-0.3f, 0.3f, 0.3f), glm::vec3(0, 0.3f, 0.3f), glm::vec3(0.3f, 0.3f, 0.3f), //front row
        glm::vec3(-0.3f, 0.3f, 0), glm::vec3(0, 0.3f, 0), glm::vec3(0.3f, 0.3f, 0), // middle row
        glm::vec3(-0.3f, 0.3f, -0.3f), glm::vec3(0, 0.3f, -0.3f), glm::vec3(0.3f, 0.3f, -0.3f), // last row
    
        // Second layer
        glm::vec3(-0.3f, 0, 0.3f), glm::vec3(0, 0, 0.3f), glm::vec3(0.3f, 0, 0.3f), //front row
        glm::vec3(-0.3f, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.3f, 0, 0), // middle row
        glm::vec3(-0.3f, 0, -0.3f), glm::vec3(0, 0, -0.3f), glm::vec3(0.3f, 0, -0.3f), // last row

        // Last layer
        glm::vec3(-0.3f, -0.3f, 0.3f), glm::vec3(0, -0.3f, 0.3f), glm::vec3(0.3f, -0.3f, 0.3f), //front row
        glm::vec3(-0.3f, -0.3f, 0), glm::vec3(0, -0.3f, 0), glm::vec3(0.3f, -0.3f, 0), // middle row
        glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(0, -0.3f, -0.3f), glm::vec3(0.3f, -0.3f, -0.3f) // last row
    };
    GLuint* textures;
};

#endif
