#version 330 core

layout (location = 0) in vec3 VPos;
layout (location = 1) in vec3 VNormal;
layout (location = 2) in vec2 VTexCoord;

out vec3 VertColor;

uniform mat4 model_sp;
uniform mat4 view_sp;
uniform mat4 proj_sp;


uniform int lighting_mode;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 ambient_color;
uniform float ambient_strength;
uniform vec3 camera_pos;
uniform float specular_strength;

uniform int shininess;


uniform vec3 model_color;


void main(){

    gl_Position = proj_sp * view_sp * model_sp * vec4(VPos, 1.0f);



    vec3 vertPos=(model_sp * vec4(VPos, 1.0f)).xyz;
    vec3 vertNormal=(model_sp * vec4(VNormal, 1.0f)).xyz;

    vec3 norm = normalize(vertNormal);
    vec3 light_dir = normalize(light_pos - vertPos);
    vec3 view_dir = normalize(camera_pos - vertPos);
    vec3 light_dir_reflect = reflect(-light_dir, norm);

    // ambient
    vec3 ambient = ambient_color * ambient_strength;

    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    // specular
    float spec = pow(max(dot(view_dir, light_dir_reflect), 0.0), shininess);
    vec3 specular = specular_strength * spec * light_color;

    vec3 light_all=((lighting_mode)&1)*(ambient + diffuse + specular)+((lighting_mode>>1)&1)*ambient+((lighting_mode>>2)&1)*diffuse+((lighting_mode>>3)&1)*specular;  


    VertColor = model_color * light_all;

}
