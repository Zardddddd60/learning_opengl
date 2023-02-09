#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture_diffuse_1;

void main()
{
    FragColor = texture(texture_diffuse_1, TexCoord);
}
