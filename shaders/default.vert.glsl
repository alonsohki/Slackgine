attribute vec2 in_TexCoord;
attribute vec3 in_Position;
attribute vec3 in_Normal;

attribute vec2 in_VertexWeight;
attribute vec2 in_Joint;
uniform bool un_Skinning;
uniform mat4 un_JointMatrices [ 64 ];

uniform mat4 un_Matrix;
uniform mat4 un_ProjectionMatrix;
uniform mat4 un_LookatMatrix;
uniform mat4 un_ModelviewMatrix;

uniform mat4 un_NormalMatrix;

varying vec3 ex_Normal;
varying vec2 ex_TexCoord;
varying vec3 ex_Color;

void main(void)
{
    vec4 in_Position4 = vec4 ( in_Position, 1.0 );
    vec4 in_Normal4 = vec4 ( in_Normal, 0.0 );

    ex_Color = vec3 ( in_VertexWeight + in_Joint, 0.0 );
    if ( un_Skinning == true )
    {
        float totalWeight = 0.0f;
        vec4 pos = vec4 ( 0.0, 0.0, 0.0, 1.0 );
        vec4 norm = vec4 ( 0.0, 0.0, 0.0, 0.0 );
        for ( int i = 0; i < 2; i++ )
        {
            int jointID = int(in_Joint[i]);
            pos += ( un_JointMatrices[jointID] * in_Position4 ) * in_VertexWeight[i];
            norm += ( un_JointMatrices[jointID] * in_Normal4 ) * in_VertexWeight[i];
            totalWeight += in_VertexWeight[i];
        }

        pos += ( 1.0 - totalWeight ) * in_Position4;
        norm += ( 1.0 - totalWeight ) * in_Normal4;
        gl_Position = un_Matrix * vec4(pos.xyz, 1.0);
        ex_Normal = (un_NormalMatrix * norm).xyz;
    }
    else
    {
        gl_Position = un_Matrix * in_Position4;
        ex_Normal = (un_NormalMatrix * in_Normal4).xyz;
    }
    normalize(ex_Normal);

    ex_TexCoord = in_TexCoord;
}
