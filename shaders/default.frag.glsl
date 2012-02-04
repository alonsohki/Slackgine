varying vec3 ex_Normal;
varying vec2 ex_TexCoord;
uniform mat4 un_LookatMatrix;
uniform vec3 un_ViewVector;

struct Material
{
    vec4  diffuse;
    vec3  ambient;
    vec3  specular;
    vec3  emission;
    float shininess;
    bool  isShadeless;
};
uniform Material un_Material;
uniform float un_TextureLevels;

struct Light
{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 position;
    vec3 direction;
};
uniform Light un_Lights [ 1 ];

uniform sampler2D un_Sampler0;

void main(void)
{
    vec4 FragColor;

    if ( un_Material.isShadeless == true )
    {
        FragColor = un_Material.diffuse;
    }
    else
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        for ( int light = 0; light < 1; ++light )
        {
            float diffuseFactor = max(-dot(ex_Normal, un_Lights[light].direction), 0.0);	
            vec4 cDiffuse = vec4 ( un_Material.diffuse.rgb * un_Lights[light].diffuse * diffuseFactor, un_Material.diffuse.a );
            vec3 cAmbient = un_Material.ambient * un_Lights[light].ambient;
            vec3 cEmission = un_Material.emission;

            vec3 halfWay = normalize(un_Lights[light].direction + un_ViewVector);
            float temp = max(-dot(ex_Normal, halfWay), 0.0);
            float specularFactor = temp / (un_Material.shininess - temp*un_Material.shininess + temp);
            vec3 cSpecular = un_Material.specular * un_Lights[light].specular * specularFactor;

            FragColor += cDiffuse + vec4 ( cAmbient + cEmission + cSpecular, 0.0 );
        }
    }

    if ( un_TextureLevels > 0.0 )
    {
        FragColor *= texture2D ( un_Sampler0, ex_TexCoord );
    }

    gl_FragColor = FragColor;
}
