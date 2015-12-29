#version 120

IN vec4 vposition;
IN vec2 vcolor;

OUT vec3 v;
OUT vec3 N;

void main()
{
    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);

    GL_TEXCOORD = gl_MultiTexCoord0;
    gl_Position = ftransform();
}
