#version 330 core

in vec2 TexCoord;

uniform sampler2D texture_diffuse_1;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture_diffuse_1, TexCoord);
}
