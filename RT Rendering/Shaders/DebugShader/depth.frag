#version 330 core

out vec4 FragColor;

uniform float far;
uniform float near;

void main(){

    float z_norm=1-near/(far-(far-near)*(gl_FragCoord.z));
    FragColor = vec4(vec3(z_norm), 1.0);

}