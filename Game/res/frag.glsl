#version 430 core

in vec3 out_fragmentPosInWorldSpace;
in vec3 out_normal;
//in vec3 out_light;
//in vec3 out_view;
//in vec2 out_UV;

uniform vec3 u_lightPos;
uniform vec3 u_matAmbient; // light color
uniform vec3 u_matDiffuse; // diffuse light color actually
uniform vec3 u_objectColor; // albedo of the obejct. TODO : sampled from a texture?
uniform vec3 u_matSpecular; // specular light color (should be the same as the light source color I suppose?)
uniform float u_matPower;
uniform vec3 u_viewerPos;

// uniform sampler2D texSampler;

// From learnopengl.com
// "We chose to do the lighting calculations in world space, 
//  but most people tend to prefer doing lighting in view space. 
//  An advantage of view space is that the viewer's position is always at (0,0,0)
//  so you already got the position of the viewer for free. 
//  However, I find calculating lighting in world space more intuitive for learning purposes.
//  If you still want to calculate lighting in view space you want to transform all the relevant 
//  vectors with the view matrix as well (don't forget to change the normal matrix too)."

void main()
{
    // ambient
    //float ambientStrength = 0.1;
    vec3 ambient = u_matAmbient;
  	
    // diffuse 
    vec3 normal = normalize(out_normal);
    vec3 lightDir = normalize(u_lightPos - out_fragmentPosInWorldSpace);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * u_matDiffuse;
            
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewerPos - out_fragmentPosInWorldSpace);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_matSpecular;  
        
    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    gl_FragColor = vec4(result, 1.0);

}
