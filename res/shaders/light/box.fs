#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularStrength;
uniform float ambientStrength;

in vec3 Normal;
// 世界坐标系的坐标
in vec3 FragPos;

void main()
{
    // float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normalizedLightDir = normalize(lightPos - FragPos);
    vec3 normalizedNormalDir = normalize(Normal);
    float diffuseStrength = max(dot(normalizedLightDir, normalizedNormalDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    // float specularStrength = 0.5;
    vec3 reflectedLightDir = reflect(-normalizedLightDir, normalizedNormalDir);
    vec3 normalizedViewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(reflectedLightDir, normalizedViewDir) ,0.0), 128);
    vec3 specular = spec * specularStrength * lightColor;

    // 本来是lightColor * objectColor
    // 现在分别是经过diffuse和ambient系数过后的lightColor
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}
