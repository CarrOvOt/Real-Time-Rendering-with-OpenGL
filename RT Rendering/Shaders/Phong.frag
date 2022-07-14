#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 VertColor;
in vec3 VertPos;
in vec3 VertNormal;

out vec4 FragColor;

uniform int lighting_mode;

uniform Material material;
uniform Light light;
uniform vec3 camera_pos;


uniform sampler2D texture_diffuse1;

void main(){
       
    vec3 norm = normalize(VertNormal);
    vec3 light_dir = normalize(light.pos - VertPos);
    vec3 view_dir = normalize(camera_pos - VertPos);
    vec3 light_dir_reflect = reflect(-light_dir, norm);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // specular
    float spec = pow(max(dot(view_dir, light_dir_reflect), 0.0), 128*material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    vec3 light_all=((lighting_mode)&1)*(ambient + diffuse + specular)+((lighting_mode>>1)&1)*ambient+((lighting_mode>>2)&1)*diffuse+((lighting_mode>>3)&1)*specular;  

    //FragColor = vec4(VertColor*(ambient + diffuse + specular), 1.0f);
    FragColor = vec4(light_all, 1.0f);
    

   
}
