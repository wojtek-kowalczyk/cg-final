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
uniform vec3 u_matSpecular;
uniform float u_matPower;

// uniform sampler2D texSampler;

void main()
{
    // Normalize the incoming normal, light and view vectors
    //vec3 N = normalize(out_normal);
    //vec3 L = normalize(out_light);
    //vec3 V = normalize(out_view);

    // Calculate R locally
    //vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    //vec3 diffuse = max(dot(N, L), 0.0) * u_matDiffuse;
    //vec3 specular = pow(max(dot(R, V), 0.0), u_matPower) * u_matSpecular;

    // Write final color to the framebuffer
    //gl_FragColor = vec4(u_matAmbient + diffuse + specular, 1.0);
    //gl_FragColor = vec4(out_normal, 1.0f);
    //gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //vec3 normal = normalize(out_normal);
    //vec3 lightDir = normalize(u_lightPos - out_fragmentPosInWorldSpace);
    //float diff = max(dot(normal, lightDir), 0.0);
    //vec3 diffuse = diff * u_matDiffuse;
    
    // TODO : separate ambient into color and strength
    //vec3 result = (u_matAmbient + diffuse) * u_objectColor;
    //FragColor = vec4(result, 1.0);
    //gl_FragColor = vec4(result, 1.0);

    // ambient
    //float ambientStrength = 0.1;
    vec3 ambient = u_matAmbient;
  	
    // diffuse 
    vec3 normal = normalize(out_normal);
    vec3 lightDir = normalize(u_lightPos - out_fragmentPosInWorldSpace);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * u_matDiffuse;
            
    vec3 result = (ambient + diffuse) * u_objectColor;
    gl_FragColor = vec4(result, 1.0);

}
