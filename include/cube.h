#ifndef CUBE_H_
#define CUBE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum FaceTexture { FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_2, FACE_TEXTURE_3, FACE_TEXTURE_4, FACE_TEXTURE_5, FACE_TEXTURE_6};

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
    
    /*
     * Draw current cube.
     * First initialize VAO & VBO if the first time drawing.
     * 
     * @param textures: textures handlers intialized and loaded.
     */
    void draw(GLuint* textures){
        if(first_draw) initDrawing();
        glBindVertexArray(VAO);

        FaceTexture currTex;

        for(int ix = 0; ix != 6; ++ix){
            currTex = face_textures[ix];
            if(currTex > FACE_TEXTURE_3) currTex = FACE_TEXTURE_0;
			glBindTexture(GL_TEXTURE_2D, textures[currTex]);
			glDrawArrays(GL_TRIANGLES, ix * 6, 6);
		}

        glBindVertexArray(0);
    }

    /*
     * Release VAO & VBO after drawing completed
     */
    void finishDrawing(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    float vertices[180] = {
	     /* vertices          texture */
	  	 // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		 // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		 // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    FaceTexture face_textures[6];
    GLuint VAO, VBO;
    bool first_draw = true;

    /*
     * initialize VAO & VBO when drawing for the first time
     */
    void initDrawing(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        first_draw = false;
    }
};

#endif
