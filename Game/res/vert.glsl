#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

out vec3 color;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

void main()
{
    color = in_color;
    gl_Position = u_p * u_v * u_m * vec4(in_position, 1.0);
}
