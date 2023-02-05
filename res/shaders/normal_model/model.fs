#version 330 core
in VS_OUT {
    vec2 TexCoord;
} fs_in;

uniform sampler2D texture_diffuse_1;

out vec3 FragColor;

void main()
{
    FragColor = texture(texture_diffuse_1, fs_in.TexCoord).rgb;
}
