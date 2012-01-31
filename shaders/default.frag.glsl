precision mediump float;
varying vec3 ex_Normal;

void main(void)
{
    gl_FragColor = vec4((ex_Normal + 1.0) * 0.5, 1.0);
}
