#version 330 core

out vec4 FragColor;

in vec3 VertColor;

void main(){
    FragColor = vec4(VertColor, 1.0f);
}
