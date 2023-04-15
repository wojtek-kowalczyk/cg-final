#version 430 core
layout (location = 0) in vec3 in_Pos;

out vec3 TexCoords;

uniform mat4 u_p;
uniform mat4 u_v;

void main()
{
    TexCoords = in_Pos;
    vec4 pos = u_p * u_v * vec4(in_Pos, 1.0);
    gl_Position = pos.xyww;
}