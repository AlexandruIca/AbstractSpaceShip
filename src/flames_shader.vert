#version 400

layout (location = 10) in vec4 aPos;
layout (location = 11) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = aPos;
    TexCoord = aTexCoord;
}