precision mediump float;
attribute vec3 in_Position;
attribute vec3 in_Normal;
attribute vec2 in_Tex2D;

uniform mat4 un_Matrix;
uniform mat4 un_ProjectionMatrix;
uniform mat4 un_LookatMatrix;
uniform mat4 un_ModelviewMatrix;

uniform mat4 un_NormalMatrix;

varying vec3 ex_Normal;

void main(void)
{
    gl_Position = un_Matrix * vec4(in_Position, 1.0);
    ex_Normal = (un_NormalMatrix * vec4(in_Normal, 1.0)).xyz;
}
