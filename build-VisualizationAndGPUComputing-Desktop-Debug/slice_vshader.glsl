#version 330
uniform mat4 mvpMatrix;
in vec4 vertexPosition;
out vec2 texCoord;
void main()
{
    // Calculate vertex position in screen space.
    gl_Position = mvpMatrix * vertexPosition;
    texCoord = vertexPosition.xy;
}
