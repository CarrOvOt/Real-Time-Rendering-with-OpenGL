#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main(){
    //FragColor = vec4(ourColor, 1.0f);
    FragColor = texture(texture_diffuse1, TexCoord);
}
