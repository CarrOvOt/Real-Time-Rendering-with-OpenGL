#version 330 core

struct Light{
    vec3 pos;   // used for point light and spot light
    vec3 dir;   // used for directed light and spot light
    float cosR1,cosR2; // used for spot light

    float power;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;
layout (location = 3) in vec3 VTagent;
layout (location = 4) in vec3 VBitangent;

out vec3 VertPos_tagent;
out vec3 VertNormal;
out vec2 VertTexCoord;

out vec3 camera_pos_tagent;

out vec3 point_light_pos_tagent;
out vec3 dir_light_pos_tagent;
out vec3 spot_light_pos_tagent;
out vec3 dir_light_dir_tagent;
out vec3 spot_light_dir_tagent;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;

uniform vec3 model_color;

uniform vec3 camera_pos;

uniform Light point_light;
uniform Light dir_light;
uniform Light spot_light;



void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(VPos, 1.0f);


    vec3 T = normalize(vec3(model_sp * vec4(VTagent,   0.0)));
    vec3 B = normalize(vec3(model_sp * vec4(VBitangent, 0.0)));
    vec3 N = normalize(vec3(model_sp * vec4(VNormal,    0.0)));
    mat3 TBN = mat3(T, B, N);
    mat3 inv_TBN = transpose(mat3(T, B, N));


    camera_pos_tagent = inv_TBN * camera_pos;
    
    point_light_pos_tagent = inv_TBN * point_light.pos;
    dir_light_pos_tagent = inv_TBN * dir_light.pos;
    spot_light_pos_tagent = inv_TBN * spot_light.pos;
    dir_light_dir_tagent = inv_TBN * dir_light.dir;
    spot_light_dir_tagent = inv_TBN * spot_light.dir;

    VertNormal = (model_sp * vec4(VNormal, 1.0f)).xyz;
    VertPos_tagent = inv_TBN * (model_sp * vec4(VPos, 1.0f)).xyz;
    VertTexCoord = VTexCoord;
}
