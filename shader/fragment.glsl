#version 330 core

in vec2 texCoords;
in vec3 fragNorm;

out vec4 fragColor;

uniform sampler2D texSampler;
// uniform vec3 cameraPos;
// uniform vec3 lightPos;

void main(){
    fragColor = texture(texSampler, texCoords);

    // vec3 ambient = vec3(0.1) * fragColor;
    // // compute diffuse lighting
    // vec3 norm = normalize(fragNorm);
    // vec3 lightDir = normalize(lightPos - fragPos);
    // vec3 diffuse = vec3(0.5) * color * max(0, dot(lightDir, norm));
    // // compute specular lighting
    // vec3 reflectDir = reflect(-lightDir, norm);
    // vec3 cameraDir = normalize(cameraPos - fragPos);
    // float spec = pow(max(0, dot(cameraDir, reflectDir)), 16);
    // vec3 specular = vec3(1.0) * vec3(0.5) * spec;

    // vec3  res = ambient + diffuse;
    // resultColor = vec4(res, 1.0f);
}