#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light{
    vec3 pos;   // used for point light and spot light
    vec3 dir;   // used for directed light and spot light
    float cosR1,cosR2; // used for spot light

    float power;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 VertPos;
in vec3 VertNormal;
in vec2 VertTexCoord;


out vec4 FragColor;

uniform Material material;

uniform Light point_light;
uniform Light dir_light;
uniform Light spot_light;


uniform vec3 camera_pos;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;



vec3 CalcDirLight(Light light, vec3 norm,vec3 view_dir){
    
    vec3 light_dir = normalize(-light.dir);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    // ambient
    vec3 ambient  = light.ambient  * texture(texture_diffuse1, VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * texture(texture_diffuse1, VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * material.shininess);
    vec3 specular = light.specular * spec * texture(texture_diffuse1, VertTexCoord).rgb;

    return (ambient + diffuse + specular) * light.power;
};

vec3 CalcPointLight(Light light, vec3 norm,vec3 view_dir){

    float dist = length(light.pos - VertPos);
    float light_strength = light.power/(dist*dist);

    vec3 light_dir = normalize(light.pos - VertPos);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    // ambient
    vec3 ambient = light.ambient * texture(texture_diffuse1,VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(texture_diffuse1,VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * material.shininess);
    vec3 specular = spec * light.specular * texture(texture_specular1,VertTexCoord).rgb;

    return (ambient + diffuse + specular)* light_strength;
   
};

vec3 CalcSpotLight(Light light, vec3 norm,vec3 view_dir){

    float dist = length(light.pos - VertPos);
    float light_strength = light.power/(dist*dist);

    vec3 light_dir = normalize(light.pos - VertPos);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    float theta = dot(light_dir, normalize(-light.dir));
    float intensity = clamp((theta - light.cosR2) / (light.cosR1-light.cosR2), 0.0, 1.0); 

    // ambient
    vec3 ambient = light.ambient * texture(texture_diffuse1,VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(texture_diffuse1,VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * material.shininess);
    vec3 specular = spec * light.specular * texture(texture_specular1,VertTexCoord).rgb;

    return (ambient + diffuse * intensity + specular * intensity)* light_strength;
   
};




void main(){

    vec3 norm = normalize(VertNormal);
    vec3 view_dir = normalize(camera_pos - VertPos);

    vec3 light_all = CalcPointLight(point_light,norm,view_dir) + CalcDirLight(dir_light,norm,view_dir) + CalcSpotLight(spot_light,norm,view_dir);

    FragColor = vec4(light_all, 1.0f);

}
