#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

uniform vec3 u_lightPos;

out vec3 out_fragmentPosInWorldSpace;
out vec3 out_normal;
out vec3 out_light;
out vec3 out_view;
out vec2 out_UV;
out mat3 out_normalMatrix;

void main()
{
    out_UV = in_uv;

    // Calculate view-space coordinate
    //vec4 P = u_v * u_m * vec4(in_position, 1.0);
    //vec4 P = u_m * vec4(in_position, 1.0);

    // Calculate normal in view-space
    //out_normal = mat3(u_v * u_m) * in_normal;
    //out_normal = mat3(u_m) * in_normal;
    out_normal = mat3(transpose(inverse(u_m))) * in_normal; // this transpose invesrse should be done on the cpu. this is to enusre correct lighting withnon-uniform scale. see here: https://learnopengl.com/Lighting/Basic-Lighting ("one last thing" section)

    // Calculate light vector
    //out_light = u_lightPos - P.xyz;

    // Calculate view vector;
    //out_view = -P.xyz;

    out_fragmentPosInWorldSpace = vec3(u_m * vec4(in_position, 1.0));

    gl_Position = u_p * u_v * vec4(out_fragmentPosInWorldSpace, 1.0);
}
