#version 330 core
out vec4 FragColor;

in vec2 VertTexCoord;

uniform sampler2D screenTexture;

void main(){


    vec3 hdrColor = texture(screenTexture, VertTexCoord).rgb;
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    FragColor = vec4(mapped,1.0f);

    //FragColor = texture(screenTexture, VertTexCoord);

}