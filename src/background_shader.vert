#version 400

layout(location = 2) in vec4 in_Position;

out vec4 ex_Color;
uniform mat4 myMatrix;
 
void main(void)
{
    gl_Position = myMatrix * in_Position;
    ex_Color = vec4(1.0);
} 