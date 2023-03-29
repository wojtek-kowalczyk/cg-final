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

struct Material {
    vec3 ambient; // these can be thought of as colors the object reflects from these types of light. or the colors of these lights.
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_mat;
uniform Light u_light; 

uniform vec3 u_viewerPos;

void main()
{
    vec3 ambient = u_light.ambient * u_mat.ambient;
  	
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_light.position - FragmentPosInWorldSpace);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * (diff * u_mat.diffuse);
            
    vec3 viewDir = normalize(u_viewerPos - FragmentPosInWorldSpace);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_mat.shininess);
    vec3 specular = u_light.specular * (spec * u_mat.specular);  
        
    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
