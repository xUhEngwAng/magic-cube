#ifndef TOOLS_H_
#define TOOLS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iomanip>

/*
 * This file contains various general tools for developing, debugging, etc.
 */

void displayMat4(const glm::mat4& m){
    std::cout << "--------------------------------" << std::endl;
    for(int ix = 0; ix != 4; ++ix){
        std::cout << "| ";
        for(int jx = 0; jx != 4; ++jx){
            // glm stores a matrix in a column-first manner
            std::cout << std::fixed << std::setw(6) << std::setprecision(3) << m[jx][ix] << ' ';
        }
        std::cout << " |" << std::endl;
    }
}

#endif
