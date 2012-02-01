attribute vec2 in_TexCoord;
attribute vec3 in_Position;
attribute vec3 in_Normal;

uniform mat4 un_Matrix;
uniform mat4 un_ProjectionMatrix;
uniform mat4 un_LookatMatrix;
uniform mat4 un_ModelviewMatrix;

uniform mat4 un_NormalMatrix;

varying vec3 ex_Normal;
varying vec2 ex_TexCoord;

void main(void)
{
    gl_Position = un_Matrix * vec4(in_Position, 1.0);
    ex_Normal = (un_NormalMatrix * vec4(in_Normal, 1.0)).xyz;
    ex_TexCoord = in_TexCoord;
}
