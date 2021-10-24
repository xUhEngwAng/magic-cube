#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main(){
    gl_Position = perspective * view * model * vec4(inPos, 1.0f);
    texCoords = inTexCoords;
}
