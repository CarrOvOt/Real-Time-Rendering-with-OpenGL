#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;

out vec3 ourColor;

void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(aPos, 1.0f);
    ourColor = vec3(1.0f, 0.5f, 0.2f);
}
