#version 330 core

out vec4 FragColor;

uniform float far;
uniform float near;


uniform vec3 outline_color;

void main(){

    FragColor = vec4(outline_color, 1.0);

}