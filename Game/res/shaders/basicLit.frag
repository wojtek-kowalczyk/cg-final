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

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

// TODO : how to pass this as uniform?
#define NUM_POINT_LIGHTS 3

uniform PointLight u_pointLights[NUM_POINT_LIGHTS]; 
uniform DirLight u_dirLight;
uniform SpotLight u_spotLight;
uniform Material u_mat;
uniform vec3 u_viewerPos;

// Function prototypes (just like in C)
vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir); 
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 outputColor = vec3(0.0);

    // TODO : I think I wanna keep ambient light separate from light sources and be a property of a scene.
    vec3 ambient = vec3(0.15, 0.15, 0.15) * texture(u_mat.diffuse, UV).rgb;

    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(u_viewerPos - FragmentPosInWorldSpace);

    outputColor += CalculateDirectionalLight(u_dirLight, normal, viewDir);

    for (int i = 0; i < NUM_POINT_LIGHTS; i++) 
    {
        outputColor += CalculatePointLight(u_pointLights[i], normal, viewDir);
    }

    outputColor += CalculateSpotLight(u_spotLight, normal, viewDir); 

    gl_FragColor = vec4(outputColor, 1.0);
}

// TODO : get rid of duplicated code in these functions

vec3 CalculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), u_mat.shininess);

    vec3 diffuse = light.diffuse * diffuseStrength * texture(u_mat.diffuse, UV).rgb;
    vec3 specular = light.specular * specularStrength * texture(u_mat.specular, UV).rgb;  

    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir) 
{
    vec3 lightDir = normalize(light.position - FragmentPosInWorldSpace);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), u_mat.shininess);
    
    vec3 diffuse = light.diffuse * diffuseStrength * texture(u_mat.diffuse, UV).rgb;
    vec3 specular = light.specular * specularStrength * texture(u_mat.specular, UV).rgb;  

    float distance    = length(light.position - FragmentPosInWorldSpace);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
        light.quadratic * (distance * distance)); 

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragmentPosInWorldSpace);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    float specularStrength = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), u_mat.shininess);
    
    vec3 diffuse = light.diffuse * diffuseStrength * texture(u_mat.diffuse, UV).rgb;
    vec3 specular = light.specular * specularStrength * texture(u_mat.specular, UV).rgb;

    float distance = length(light.position - FragmentPosInWorldSpace);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse *= attenuation;
    specular *= attenuation;
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    diffuse *= intensity;
    specular *= intensity;
    
    return (diffuse + specular);
}