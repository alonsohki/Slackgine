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

void main(void)
{
    ivec2 iJoint = ivec2 ( in_Joint );
    vec4 in_Position4 = vec4 ( in_Position, 1.0 );
    vec4 position = in_Position4;

    if ( un_Skinning == true )
    {
        position += un_JointMatrices[iJoint[0]] * in_Position4 * in_VertexWeight[0];
        position += un_JointMatrices[iJoint[1]] * in_Position4 * in_VertexWeight[1];
    }

    gl_Position = un_Matrix * position;

    ex_Normal = (un_NormalMatrix * vec4(in_Normal, 1.0)).xyz;
    ex_TexCoord = in_TexCoord;
}
