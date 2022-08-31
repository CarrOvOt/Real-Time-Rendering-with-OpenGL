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

in vec2 VertTexCoord;

uniform sampler2D albedo_texture;
uniform sampler2D normal_texture;
uniform sampler2D position_texture;
uniform sampler2D metallic_roughness_texture;

uniform vec3 camera_pos;
uniform vec3 camera_dir;

const float bloom_threshold=1.0f;

// direct light
const int LightNR = 32;
uniform Light point_light[LightNR];
uniform Light dir_light[LightNR];
uniform Light spot_light[LightNR];

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;


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

vec3 KD_star(vec3 v, vec3 n, vec3 f0, float alpha, float m){

    float vn=max(dot(v, n), 0.0);
    vec3 fr = f0 + (max(vec3(1-alpha),f0)-f0)*pow(1-vn,5);

    return (1-fr)*(1-m);
}


vec3 PBR_directed_light(Light light, vec3 view_dir, vec3 norm, vec3 albedo, vec3 F0, float metallic, float roughness){

    vec3 light_dir = - normalize(light.dir);
    vec3 halfway_dir = normalize(light_dir+view_dir);

    float D=DGGX(norm,halfway_dir,max(roughness,0.002));
    vec3 F=FSchlick(view_dir,halfway_dir,F0);
    float G=GSmith(light_dir,norm,view_dir,roughness);

    vec3 diffuse = albedo/PI * (vec3(1.0)-F) * (1-metallic);
    vec3 specular = D*F*G/(4*max(dot(norm,light_dir),0.00001)*max(dot(norm,view_dir),0.00001));

    vec3 radiance = light.color * light.power * dot(norm ,light_dir);

    return max((diffuse + specular) * radiance,0.0f);
}

vec3 PBR_point_light(vec3 VertPos,Light light, vec3 view_dir, vec3 norm,vec3 albedo,vec3 F0,float metallic,float roughness){

    vec3 light_dir = normalize(light.pos - VertPos);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    float D=DGGX(norm,halfway_dir,max(roughness,0.002));
    vec3 F=FSchlick(view_dir,halfway_dir,F0);
    float G=GSmith(light_dir,norm,view_dir,roughness);

    vec3 diffuse = albedo/PI * (vec3(1.0)-F) * (1-metallic);
    vec3 specular = D*F*G/(4*max(dot(norm,light_dir),0.00001)*max(dot(norm,view_dir),0.00001));

    float dist = length(light.pos - VertPos);
    float light_strength = light.power/(dist*dist);

    vec3 radiance = light.color * light_strength * dot(norm ,light_dir);

    return (diffuse + specular) * radiance;
}

vec3 PBR_spot_light(vec3 VertPos,Light light, vec3 view_dir, vec3 norm,vec3 albedo,vec3 F0,float metallic,float roughness){

    vec3 light_dir = normalize(light.pos - VertPos);
    vec3 halfway_dir = normalize(light_dir + view_dir);

    float D=DGGX(norm,halfway_dir,max(roughness,0.002));
    vec3 F=FSchlick(view_dir,halfway_dir,F0);
    float G=GSmith(light_dir,norm,view_dir,roughness);

    vec3 diffuse = albedo/PI * (vec3(1.0)-F) * (1-metallic);
    vec3 specular = D*F*G/(4*max(dot(norm,light_dir),0.00001)*max(dot(norm,view_dir),0.00001));

    float dist = length(light.pos - VertPos);
    float light_strength = light.power/(dist*dist);
    float theta = dot(light_dir, normalize(-light.dir));
    float intensity = clamp((theta - light.cosR2) / (light.cosR1-light.cosR2), 0.0, 1.0); 

    vec3 radiance = light.color * light_strength * dot(norm ,light_dir) * intensity;

    return (diffuse + specular) * radiance;
}


void main(){

	// requirements
	vec3 albedo = texture(albedo_texture,VertTexCoord).rgb;

    vec2 mr=texture(metallic_roughness_texture,VertTexCoord).rg;
	float metallic=mr.x;
	float roughness=mr.y;

	vec3 norm = texture(normal_texture,VertTexCoord).rgb;;
	vec3 vertpos = texture(position_texture,VertTexCoord).rgb;;

	// compute each frame
	vec3 F0 = mix(vec3(0.04f), albedo, metallic);

	vec3 view_dir = normalize(camera_pos-vertpos);
	//vec3 view_dir = normalize(-camera_dir); // use this if camera is orthogonal
	vec3 reflect_view = reflect(-view_dir,norm);


    // direct lighting
    FragColor=vec4(0.0f); 
    for(int i=0;i<LightNR;++i){
        FragColor += vec4(PBR_directed_light(dir_light[i],view_dir,norm,albedo,F0,metallic,roughness),1.0f);
        FragColor += vec4(PBR_point_light(vertpos,point_light[i],view_dir,norm,albedo,F0,metallic,roughness),1.0f);
        FragColor += vec4(PBR_spot_light(vertpos,spot_light[i],view_dir,norm,albedo,F0,metallic,roughness),1.0f);
    }


    // IBL
    vec3 diffuse_ibl = KD_star(view_dir,norm,F0,roughness,metallic) * albedo / PI * texture(irradianceMap,norm).rgb;
    vec4 lut_val = texture(brdfLUT,vec2(max(dot(norm, view_dir), 0.0), roughness));
    vec3 specular_ibl = textureLod(prefilterMap, reflect_view, roughness * 4.0f).rgb * (F0 * lut_val.r + lut_val.g);

    FragColor += vec4(diffuse_ibl + specular_ibl, 1.0);

    // bloom
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > bloom_threshold)
        BloomColor = vec4(FragColor.rgb, 1.0);

}