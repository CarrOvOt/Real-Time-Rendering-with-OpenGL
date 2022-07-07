#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;

out vec3 ourColor;
out vec2 TexCoord;

void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(aPos, 1.0f);
    ourColor = vec3(1.0f, 0.5f, 0.2f);
    TexCoord=aTexCoord;
}
