#version 330
uniform sampler2D colorMappingTexture;
in vec2 texCoord;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture(colorMappingTexture, texCoord);
}
