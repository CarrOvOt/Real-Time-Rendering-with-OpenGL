#version 330 core

in vec3 VertColor;

out vec4 FragColor;


uniform sampler2D texture_diffuse1;

void main(){
       
    FragColor = vec4(VertColor, 1.0f);
    
}
