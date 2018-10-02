#version 430
out vec4 Fragment;
in vec2 fTex;
uniform sampler2D Tex;
void main()
{
    Fragment = texture(Tex, fTex);
}