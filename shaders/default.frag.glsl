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
    int   textureLevels;
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

uniform sampler2D un_Sampler0;

void doLight(int light)
{
    float diffuseFactor = max(-dot(ex_Normal, un_Lights[light].direction), 0.0);	
    vec4 cDiffuse = vec4 ( un_Material.diffuse.rgb * un_Lights[light].diffuse * diffuseFactor, un_Material.diffuse.a );
    vec3 cAmbient = un_Material.ambient * un_Lights[light].ambient;
    vec3 cEmission = un_Material.emission;
    
    vec3 halfWay = normalize(un_Lights[light].direction + un_ViewVector);
    float temp = max(-dot(ex_Normal, halfWay), 0.0);
    float specularFactor = temp / (un_Material.shininess - temp*un_Material.shininess + temp);
    vec3 cSpecular = un_Material.specular * un_Lights[light].specular * specularFactor;

    gl_FragColor += cDiffuse + vec4 ( cAmbient + cEmission + cSpecular, 0.0 );
}

void mapTexture ( sampler2D sampler )
{
    vec4 texel = texture2D ( sampler, ex_TexCoord );
    gl_FragColor = vec4 ( gl_FragColor.rgb * texel.rgb, gl_FragColor.a * texel.a );
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

    if ( un_Material.textureLevels > 0 )
        mapTexture ( un_Sampler0 );
}
