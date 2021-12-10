#version 330 core

in vec2 texCoords;
in vec3 fragNorm;
in vec3 fragPos;

out vec4 resultColor;

uniform sampler2D texSampler;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;

void main(){
    vec3 fragColor = vec3(texture(texSampler, texCoords));

    vec3 ambient = light_ambient * fragColor;
    // compute diffuse lighting
    vec3 norm = normalize(fragNorm);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 diffuse = light_diffuse * fragColor * max(0, dot(lightDir, norm));

    vec3  res = ambient + diffuse;
    resultColor = vec4(res, 1.0f);
}