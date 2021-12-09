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
enum RotateLayer {LAYER_ONE, LAYER_TWO, LAYER_THREE, LAYER_FOUR, LAYER_FIVE, LAYER_SIX, LAYER_ALL, LAYER_NONE};

class MagicCube {
public:
    /*
     * MagicCube default constructor.
     * Intialize texture image & model matrix for each cube
     */
    MagicCube(): rank(3) { init(); }
    MagicCube(int rank_): rank(rank_) { init(); }

    void init(){
        /* Initilize face textures & position of each cube */
        // ---------------------------------------
        glm::mat4 model;
        glm::vec3 curr_pos;
        int curr_ix;
        float cube_length = length / rank;
        cubes = std::vector<Cube>(rank * rank * rank);

        for(int layer = 0; layer != rank; ++layer){
            for(int row = 0; row != rank; ++row){
                for(int col = 0; col != rank; ++col){
                    curr_pos = glm::vec3(col+0.5f, layer+0.5f, -row-0.5f) * glm::vec3(cube_length);                    
                    model = glm::translate(glm::mat4(1.0f), curr_pos);
                    model = glm::scale(model, glm::vec3(cube_length));
                    curr_ix = rank * (layer * rank + row) + col;
                    cubes[curr_ix].setModel(model);

                    if(layer == 0) cubes[curr_ix].setFaceTexture(FACE_BUTTOM, FACE_TEXTURE_5);
                    if(layer == rank-1) cubes[curr_ix].setFaceTexture(FACE_TOP, FACE_TEXTURE_6);
                    if(row == 0) cubes[curr_ix].setFaceTexture(FACE_FRONT, FACE_TEXTURE_2);
                    if(row == rank-1) cubes[curr_ix].setFaceTexture(FACE_BACK, FACE_TEXTURE_1);
                    if(col == 0) cubes[curr_ix].setFaceTexture(FACE_LEFT, FACE_TEXTURE_3);
                    if(col == rank-1) cubes[curr_ix].setFaceTexture(FACE_RIGHT, FACE_TEXTURE_4);
                }
            }
        }
    }

    void setRank(int rank_){
        rank = rank_;
        init();
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
        glm::vec3 axis = glm::vec3(1.0f, 0, 0);
        glm::vec3 center;
        glm::mat4 model;

        if(state == ROTATE_X){
            axis = glm::vec3(1.0f, 0, 0);
            center = glm::vec3(0, 0.5f, -0.5f) * glm::vec3(length);
        }
        if(state == ROTATE_Y){
            axis = glm::vec3(0, 1.0f, 0);
            center = glm::vec3(0.5f, 0, -0.5f) * glm::vec3(length);
        }
        if(state == ROTATE_Z){
            axis = glm::vec3(0, 0, 1.0f);
            center = glm::vec3(0.5f, 0.5f, 0) * glm::vec3(length);
        }

        for(int ix = 0; ix != cubes.size(); ++ix){
            if(cube_qualified(ix, state, layer))
                cubes[ix].draw(shader, textures, axis, center, angle);
            else
                cubes[ix].draw(shader, textures, axis, center, 0);
        }
    }
    
    /*
     * Rotate the magic cube as a whole
     *
     * @param axis: rotation axis
     * @param angle: rotation angle, expressed in degrees
     */
    void rotate(RotateState state, RotateLayer layer, float angle){
        glm::vec3 axis;
        glm::vec3 center;

        if(state == ROTATE_X){
            axis = glm::vec3(1.0f, 0, 0);
            center = glm::vec3(0, 0.5f, -0.5f) * glm::vec3(length);
        }
        if(state == ROTATE_Y){
            axis = glm::vec3(0, 1.0f, 0);
            center = glm::vec3(0.5f, 0, -0.5f) * glm::vec3(length);
        }
        if(state == ROTATE_Z){
            axis = glm::vec3(0, 0, 1.0f);
            center = glm::vec3(0.5f, 0.5f, 0) * glm::vec3(length);
        }

        for(int ix = 0; ix != cubes.size(); ++ix){
            if(cube_qualified(ix, state, layer)){
                cubes[ix].rotate(axis, center, angle);
            }
        }
    }

    bool cube_qualified(int cube_ix, const RotateState state, const RotateLayer layer){
        if(state == ROTATE_NONE) return false;
        if(layer == LAYER_ALL) return true;

        float cube_length = length / rank;
        float layer_center = (0.5 + static_cast<int>(layer)) * cube_length;

        bool qualified;
        switch(state){
            case ROTATE_X:
                qualified = fabs(cubes[cube_ix].getCenter().x - layer_center) < 1e-5f;
            break;
            case ROTATE_Y:
                qualified = fabs(cubes[cube_ix].getCenter().y - layer_center) < 1e-5f;
            break;
            case ROTATE_Z:
                qualified = fabs(cubes[cube_ix].getCenter().z + layer_center) < 1e-5f;
            break;
            default:
            break;
        }
        return qualified;
    }

    RotateLayer getLayer(float coord){
        RotateLayer layer;
        float cube_length = length / rank;
        float center;

        for(int ix = 0; ix != rank; ++ix){
            center = (ix + 0.5) * cube_length;
            if(fabs(coord - center) < cube_length / 2){
                return RotateLayer(ix);
            }
        }

        return LAYER_NONE;
    }

    bool hit(const Ray& ray, double t_min, double t_max, HitRecord& rec){
        bool ishit = false;
        for(int ix = 0; ix != cubes.size(); ++ix){
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
    int rank;
    float length = 1.2;
    std::vector<Cube> cubes;
    GLuint* textures;
};

#endif
