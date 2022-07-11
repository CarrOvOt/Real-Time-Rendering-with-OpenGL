#version 330 core

in vec3 VertColor;
in vec3 VertPos;
in vec3 VertNormal;

out vec4 FragColor;

uniform int lighting_mode;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 ambient_color;
uniform float ambient_strength;
uniform vec3 camera_pos;
uniform float specular_strength;

uniform int shininess;


uniform sampler2D texture_diffuse1;

void main(){
       
    vec3 norm = normalize(VertNormal);
    vec3 light_dir = normalize(light_pos - VertPos);
    vec3 view_dir = normalize(camera_pos - VertPos);
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

    //FragColor = vec4(VertColor*(ambient + diffuse + specular), 1.0f);
    FragColor = vec4(VertColor*light_all, 1.0f);
    
    

   
}
