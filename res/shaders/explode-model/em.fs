#version 330 core
struct Material
{
    sampler2D texture_diffuse_1;
    sampler2D texture_specular_1;
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform Material material;

void main()
{
    FragColor = vec4(texture(material.texture_diffuse_1, TexCoords).rgb, 1.0);
}

