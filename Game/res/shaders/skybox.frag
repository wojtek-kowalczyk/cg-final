#version 330 core

in vec3 TexCoords;

uniform samplerCube u_skybox;

out vec4 fragColor;

void main()
{    
    fragColor = texture(u_skybox, TexCoords);
}