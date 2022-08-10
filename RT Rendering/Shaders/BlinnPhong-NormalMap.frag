#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;


struct Light{
    vec3 pos;   // used for point light and spot light
    vec3 dir;   // used for directed light and spot light
    float cosR1,cosR2; // used for spot light

    float power;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 VertPos_tagent;
in vec3 VertNormal;
in vec2 VertTexCoord;


uniform float shininess;

uniform Light point_light;
uniform Light dir_light;
uniform Light spot_light;


in vec3 camera_pos_tagent;

in vec3 point_light_pos_tagent;
in vec3 dir_light_pos_tagent;
in vec3 spot_light_pos_tagent;
in vec3 dir_light_dir_tagent;
in vec3 spot_light_dir_tagent;

uniform float bloom_threshold;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;



vec3 CalcDirLight(Light light, vec3 norm,vec3 view_dir){
    
    vec3 light_dir = normalize(-dir_light_dir_tagent);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    // ambient
    vec3 ambient  = light.ambient  * texture(texture_diffuse1, VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * texture(texture_diffuse1, VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * shininess);
    vec3 specular = light.specular * spec * texture(texture_diffuse1, VertTexCoord).rgb;

    return (ambient + diffuse + specular) * light.power;
};

vec3 CalcPointLight(Light light, vec3 norm,vec3 view_dir){

    float dist = length(point_light_pos_tagent - VertPos_tagent);
    float light_strength = light.power/(dist*dist);

    vec3 light_dir = normalize(point_light_pos_tagent - VertPos_tagent);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    // ambient
    vec3 ambient = light.ambient * texture(texture_diffuse1,VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(texture_diffuse1,VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * shininess);
    vec3 specular = spec * light.specular * texture(texture_specular1,VertTexCoord).rgb;

    return (ambient + diffuse + specular)* light_strength;
   
};

vec3 CalcSpotLight(Light light, vec3 norm,vec3 view_dir){

    float dist = length(spot_light_pos_tagent - VertPos_tagent);
    float light_strength = light.power/(dist*dist);

    vec3 light_dir = normalize(spot_light_pos_tagent - VertPos_tagent);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    float theta = dot(light_dir, normalize(-spot_light_dir_tagent));
    float intensity = clamp((theta - light.cosR2) / (light.cosR1-light.cosR2), 0.0, 1.0); 

    // ambient
    vec3 ambient = light.ambient * texture(texture_diffuse1,VertTexCoord).rgb;
    // diffuse
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture(texture_diffuse1,VertTexCoord).rgb;
    // specular
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 128 * shininess);
    vec3 specular = spec * light.specular * texture(texture_specular1,VertTexCoord).rgb;

    return (ambient + diffuse * intensity + specular * intensity)* light_strength;
   
};




void main(){

    vec3 norm = texture(texture_normal1, VertTexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);


    vec3 view_dir = normalize(camera_pos_tagent - VertPos_tagent);

    vec3 light_all = CalcPointLight(point_light,norm,view_dir) + CalcDirLight(dir_light,norm,view_dir) + CalcSpotLight(spot_light,norm,view_dir);

    FragColor = vec4(light_all, 1.0f);
    //FragColor =vec4(texture(texture_normal1,VertTexCoord).rgb,1.0f);
    //FragColor =vec4(VertNormal,1.0f);


    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > bloom_threshold)
        BloomColor = vec4(FragColor.rgb, 1.0);


}
