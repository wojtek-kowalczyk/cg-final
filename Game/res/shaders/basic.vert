#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_color;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;
uniform vec2 u_UVScale;

out vec3 FragmentPosInWorldSpace;
out vec3 Normal;
out vec2 UV;
out vec3 Color;

void main()
{
    UV = in_uv * u_UVScale;
    Color = in_color;

    // this transpose invesrse should be done on the cpu. 
    // this is to enusre correct lighting withnon-uniform scale. 
    // see here: https://learnopengl.com/Lighting/Basic-Lighting ("one last thing" section)
    Normal = mat3(transpose(inverse(u_m))) * in_normal; 

    FragmentPosInWorldSpace = vec3(u_m * vec4(in_position, 1.0));

    gl_Position = u_p * u_v * vec4(FragmentPosInWorldSpace, 1.0);
}
