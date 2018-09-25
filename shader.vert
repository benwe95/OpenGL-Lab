#version 430
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
in vec3 vCol;
in vec2 vPos;
out vec3 color;
void main()
{
    gl_Position = Projection * View * Model * vec4(vPos, 0.0, 1.0);
    color = vCol;
}