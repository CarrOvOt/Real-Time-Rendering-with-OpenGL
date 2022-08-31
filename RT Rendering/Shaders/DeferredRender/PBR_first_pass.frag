#version 330 core

layout (location = 0) out vec3 albedo_texture;
layout (location = 1) out vec3 normal_texture;
layout (location = 2) out vec3 position_texture;
layout (location = 3) out vec2 metallic_roughness_texture;


in vec3 VertPos;
in vec3 VertNormal;
in vec2 VertTexCoord;
in mat3 TBN;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;



void main(){
	

	vec3 norm = TBN * normalize(texture(texture_normal1,VertTexCoord).rgb * 2.0 - 1.0);

	float metallic=texture(texture_metallic1,VertTexCoord).b;
    float roughness=texture(texture_roughness1,VertTexCoord).g;


	albedo_texture=texture(texture_diffuse1,VertTexCoord).rgb;
	normal_texture=norm;
	position_texture=VertPos;
	metallic_roughness_texture=vec2(metallic,roughness);

}