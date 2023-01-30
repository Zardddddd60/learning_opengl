#version 330 core

in vec3 Normal;
in vec3 WorldPos;
uniform samplerCube texture1;
uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    vec3 normalizedInDIr = normalize(WorldPos - cameraPos);
    // vec3 reflectedDir = reflect(normalizedInDIr, normalize(Normal));
    // FragColor = vec4(texture(texture1, reflectedDir).rgb, 1.0);
    vec3 refractedDir = refract(normalizedInDIr, normalize(Normal), 1.00 / 1.52);
    FragColor = vec4(texture(texture1, refractedDir).rgb, 1.0);
}
