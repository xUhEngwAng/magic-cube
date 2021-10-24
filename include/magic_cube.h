#ifndef MAGIC_CUBE_H
#define MAGIC_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.h"

class MagicCube {
public:
    MagicCube() {
        /* Initilize face textures of each cube */
        // ---------------------------------------
        // First layer, front row
        cubes[0].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[1].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[2].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_1, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_2);
        // First layer, middle row
        cubes[3].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[4].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[5].setFaceTexture(FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_2);
        // First layer, last row
        cubes[6].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_3, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[7].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_2);
        cubes[8].setFaceTexture(FACE_TEXTURE_2, FACE_TEXTURE_0, FACE_TEXTURE_0, FACE_TEXTURE_4, FACE_TEXTURE_0, FACE_TEXTURE_2);

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
    }
private:
    Cube cubes[27];
    glm::vec4 model_matrices[27];
}

#endif
