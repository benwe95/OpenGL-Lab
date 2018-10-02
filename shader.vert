#version 430
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
in vec3 vPos;
in vec2 vTex;
out vec2 fTex;
void main()
{
    fTex = vTex;
    gl_Position = Projection * View * Model * vec4(vPos, 1.0);
}