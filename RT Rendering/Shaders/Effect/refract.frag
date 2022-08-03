#version 330 core


in vec3 VertNormal;
in vec3 VertPos;
in vec2 VertTexCoord;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 camera_pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform samplerCube skybox;



void main(){

    float ratio = 1.00 / 1.52;

    vec3 norm = texture(texture_normal1, VertTexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = TBN * norm;

    vec3 view_dir = - normalize(camera_pos - VertPos);

    vec3 R = refract(view_dir, norm,ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);

}
