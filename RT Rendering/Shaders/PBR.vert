#version 330 core

layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;
layout (location = 3) in vec3 VTagent;
layout (location = 4) in vec3 VBitangent;

out vec3 VertPos;
out vec3 VertNormal;
out vec2 VertTexCoord;
out mat3 TBN;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;


void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(VPos, 1.0f);

    vec3 T = normalize(vec3(model_sp * vec4(VTagent,   0.0)));
    vec3 B = normalize(vec3(model_sp * vec4(VBitangent, 0.0)));
    vec3 N = normalize(vec3(model_sp * vec4(VNormal,    0.0)));
    TBN = mat3(T, B, N);

    VertPos =  (model_sp * vec4(VPos, 1.0f)).xyz;
    VertNormal = (model_sp * vec4(VNormal, 0.0f)).xyz;
    VertTexCoord = VTexCoord;
}
