#version 330 core
out vec4 FragColor;

in vec2 VertTexCoord;

uniform bool useBloom;

uniform sampler2D screenTexture;
uniform sampler2D screenBloomTexture;


void main(){


    vec3 hdrColor = texture(screenTexture, VertTexCoord).rgb;
    if(useBloom)
        hdrColor += texture(screenBloomTexture, VertTexCoord).rgb;
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    FragColor = vec4(mapped,1.0f);

    //FragColor = texture(screenBloomTexture, VertTexCoord);

}