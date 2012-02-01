varying vec3 ex_Normal;
uniform mat4 un_LookatMatrix;
uniform vec3 un_ViewVector;

struct Material
{
    vec4 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emission;
    float shininess;
    bool isShadeless;
};
uniform Material un_Material;

struct Light
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 position;
    vec3 direction;
};
uniform Light un_Lights [ 1 ];

void doLight(int light)
{
    float diffuseFactor = max(-dot(ex_Normal, un_Lights[light].direction), 0);
    vec4 cDiffuse = un_Material.diffuse * vec4(un_Lights[light].diffuse, 1.0) * diffuseFactor;
    vec3 cAmbient = un_Material.ambient * un_Lights[light].ambient;
    vec3 cEmission = un_Material.emission;
    
    vec3 halfWay = normalize(un_Lights[light].direction + un_ViewVector);
    float temp = max(-dot(ex_Normal, halfWay), 0);
    float specularFactor = temp / (un_Material.shininess - temp*un_Material.shininess + temp);
    vec3 cSpecular = un_Material.specular * un_Lights[light].specular * specularFactor;

    gl_FragColor += cDiffuse + vec4 ( cAmbient + cEmission + cSpecular, 1.0 );
}

void main(void)
{
    if ( un_Material.isShadeless == true )
    {
        gl_FragColor = un_Material.diffuse;
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        doLight(0);
    }
}
