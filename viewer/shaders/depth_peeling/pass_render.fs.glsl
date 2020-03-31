#version 330 core

out vec4 color;

uniform bool firstPass;
uniform float width;
uniform float height;
uniform sampler2D depthTexture;
uniform vec4 inputColor;

void main()
{
    color = inputColor;
    
    if (!firstPass)
    {
        vec2 texCoord = vec2(float(gl_FragCoord.x) / width, float(gl_FragCoord.y) / height);
        float maxDepth = texture(depthTexture,texCoord).r;
        
        if(gl_FragCoord.z <= maxDepth)
        {
            discard;
        }
    }
}
