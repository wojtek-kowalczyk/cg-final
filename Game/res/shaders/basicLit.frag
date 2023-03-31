#version 430 core

// From learnopengl.com
// "We chose to do the lighting calculations in world space, 
//  but most people tend to prefer doing lighting in view space. 
//  An advantage of view space is that the viewer's position is always at (0,0,0)
//  so you already got the position of the viewer for free. 
//  However, I find calculating lighting in world space more intuitive for learning purposes.
//  If you still want to calculate lighting in view space you want to transform all the relevant 
//  vectors with the view matrix as well (don't forget to change the Normal matrix too)."

in vec3 FragmentPosInWorldSpace;
in vec3 Normal;
in vec2 UV;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material u_mat;
uniform Light u_light; 

uniform vec3 u_viewerPos;

void main()
{
    vec3 ambient = u_light.ambient * texture(u_mat.diffuse, UV).rgb;
  	
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_light.position - FragmentPosInWorldSpace);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * texture(u_mat.diffuse, UV).rgb;
            
    vec3 viewDir = normalize(u_viewerPos - FragmentPosInWorldSpace);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_mat.shininess);
    vec3 specular = u_light.specular * spec * texture(u_mat.specular, UV).rgb;  
    
    float distance    = length(u_light.position - FragmentPosInWorldSpace);
    float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + 
        u_light.quadratic * (distance * distance)); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
