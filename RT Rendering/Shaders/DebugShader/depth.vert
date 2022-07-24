#version 330 core

layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;


uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;


void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(VPos, 1.0f);

}
