uniform sampler2D colorMappingTexture;
smooth in vec2 texCoord;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = texture2D(colorMappingTexture, texCoord);
}
