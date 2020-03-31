#version 330 core

in vec2 textureCoordinates;
out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;

void main()
{
    vec4 depth = texture(depthTexture, textureCoordinates);
    if(depth.r >= 1)
    {
        discard;
    }

    color = texture(colorTexture, textureCoordinates);
}
