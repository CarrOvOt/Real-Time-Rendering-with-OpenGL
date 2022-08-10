#version 330 core

struct Light{
    vec3 pos;   // used for point light and spot light
    vec3 dir;   // used for directed light and spot light
    float cosR1,cosR2; // used for spot light

    float power;

    vec3 color;
};


layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec3 VertPos;
in vec3 VertNormal;
in vec2 VertTexCoord;
in mat3 TBN;

uniform Light point_light;
uniform Light dir_light;
uniform Light spot_light;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform vec3 camera_pos;
uniform vec3 camera_dir;
uniform float bloom_threshold;

uniform vec3  albedo; // color
uniform float roughness;
uniform float metallic;


float PI = 3.14159265359f;

float DGGX(vec3 n,vec3 h, float alpha){
    
    float a2=alpha*alpha;
    float nh=max(dot(n, h), 0.0);
    float b=nh*nh*(a2-1)+1;

    return a2/(PI*b*b);
}

vec3 FSchlick(vec3 v,vec3 h, vec3 f0){
    
    float vh=max(dot(v, h), 0.0);

    return f0+(1-f0)*(1-pow(vh,5));
}

float GSchlickGGX(vec3 n, vec3 x, float k){
    
    float nx=max(dot(n, x), 0.0);

    return nx/(nx*(1-k)+k);
}

float GSmith(vec3 l, vec3 n, vec3 v,float alpha){
    
    float k=(alpha+1)*(alpha+1)/8;

    return GSchlickGGX(n,l,k)*GSchlickGGX(n,v,k);
}

void main(){
    
    vec3 norm = normalize(VertNormal);
    vec3 light_dir = - normalize(dir_light.dir); // directed light
    //vec3 view_dir = normalize(camera_pos-VertPos);
    vec3 view_dir = normalize(-camera_dir); // use this if camera is orthogonal
    vec3 halfway_dir = normalize(light_dir+view_dir);

    vec3 radiance = dir_light.color * dir_light.power * dot(norm ,light_dir);

    vec3 F0 = mix(vec3(0.04f), albedo, metallic);

    float D=DGGX(norm,halfway_dir,max(roughness,0.002));
    vec3 F=FSchlick(view_dir,halfway_dir,F0);
    float G=GSmith(light_dir,norm,view_dir,roughness);

    vec3 diffuse = albedo/PI * (vec3(1.0)-F) * (1-metallic);
    vec3 specular = D*F*G/(4*max(dot(norm,light_dir),0.0001)*max(dot(norm,view_dir),0.0001));

    FragColor = vec4((diffuse + specular) * radiance,1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > bloom_threshold)
        BloomColor = vec4(FragColor.rgb, 1.0);
}
