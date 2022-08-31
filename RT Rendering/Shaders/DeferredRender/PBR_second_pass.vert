#version 330 core

layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;

out vec2 VertTexCoord;



void main(){

    gl_Position = vec4(VPos.x, VPos.y, 0.0f, 1.0f);

    VertTexCoord = VTexCoord;
}
