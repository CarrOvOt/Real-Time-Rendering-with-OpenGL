#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view_sp;
uniform mat4 proj_sp;

void main(){

    TexCoords = aPos;
    vec4 pos = proj_sp * view_sp * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}  