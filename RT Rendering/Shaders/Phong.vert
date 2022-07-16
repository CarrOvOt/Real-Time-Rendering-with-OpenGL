#version 330 core

layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;

out vec3 VertColor;
out vec3 VertPos;
out vec3 VertNormal;
out vec2 VertTexCoord;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;

uniform vec3 model_color;


void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(VPos, 1.0f);

    VertColor = model_color;
    VertNormal = (model_sp * vec4(VNormal, 1.0f)).xyz;
    VertPos = (model_sp * vec4(VPos, 1.0f)).xyz;
    VertTexCoord = VTexCoord;
}
