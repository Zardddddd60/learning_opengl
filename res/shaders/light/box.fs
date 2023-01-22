#version 330 core
struct Material
{
    sampler2D diffuse;
    // vec3 specular;
    sampler2D specular;
    float shininess;
};

// 已经完成了系数与光相乘
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // vec3 position;
    vec3 direction;
};

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 Normal;
// 世界坐标系的坐标
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    // float ambientStrength = 0.1;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // 原来根据光源的点和着色点得到方向，现在直接就是方向
    // vec3 normalizedLightDir = normalize(lightDir - FragPos);
    vec3 normalizedLightDir = normalize(-light.direction);
    vec3 normalizedNormalDir = normalize(Normal);
    float diffuseStrength = max(dot(normalizedLightDir, normalizedNormalDir), 0.0);
    vec3 diffuse = (diffuseStrength * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    // float specularStrength = 0.5;
    vec3 reflectedLightDir = reflect(-normalizedLightDir, normalizedNormalDir);
    vec3 normalizedViewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(reflectedLightDir, normalizedViewDir) ,0.0), material.shininess);
    vec3 specular = spec * texture(material.specular, TexCoords).rgb * light.specular;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}
