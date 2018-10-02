#version 430
uniform mat4 MVP;
in vec3 vPos;
in vec2 vTex;
out vec2 fTex;
void main()
{
    fTex = vTex;
    gl_Position = MVP * vec4(vPos, 1.0);
}