#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;

uniform vec3 pure_color;

out vec3 VertColor;

void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(aPos, 1.0f);
    VertColor = pure_color;
}
