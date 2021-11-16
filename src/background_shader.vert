#version 400

layout(location = 2) in vec4 in_Position;
layout(location = 3) in vec2 in_Center;

out vec4 ex_Color;
uniform mat4 myMatrix;
uniform mat4 starMatrix;
 
void main(void)
{
    vec4 at_center = vec4(in_Position.xy - in_Center, in_Position.zw);
    at_center = starMatrix * at_center;
    at_center = vec4(at_center.xy + in_Center.xy, at_center.zw);

    gl_Position = myMatrix * at_center;
    ex_Color = vec4(1.0);
} 