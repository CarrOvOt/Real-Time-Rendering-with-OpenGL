#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

void main(){

    TexCoords = ((aPos+1.0f)/2).xy;  
    gl_Position =  vec4(aPos, 1.0);
}