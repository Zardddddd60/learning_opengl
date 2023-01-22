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

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quandratic;
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
    // 着色点到光源的方向，射向光源
    vec3 normalizedFragmentLightPositionDirection =
        normalize(light.position - FragPos);
    // 射向光源的方向
    vec3 normalizedToLightPositionDirection =
        normalize(-light.direction);
    // 判断两者的夹角，在cutOff内，才走正常的Phong shading
    // 否则只有ambient
    float theta = dot(
        normalizedFragmentLightPositionDirection,
        normalizedToLightPositionDirection
    );

    // 注意theta的cutOff都是余弦值，theta > cutOff时，在圆的范围内
    float epsilon = light.cutOff - light.outerCutOff;
    if (theta > light.outerCutOff)
    {
        // 计算光圈范围内的衰减
        float cosDiff = theta - light.outerCutOff;
        float flashLightAttenuation = clamp(cosDiff / epsilon, 0.0, 1.0);

        float distance = length(light.position - FragPos);
        float distanceAttenuation = 1 / (
            light.constant +
            light.linear * distance +
            light.quandratic * distance * distance
        );
        // 在flashLight这里不受距离衰减，否则远距离时，环境光在light内反而更小
        vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

        // 求diffuse
        vec3 normalizedNormalDir = normalize(Normal);
        float diffuseStrength = max(dot(normalizedFragmentLightPositionDirection, normalizedNormalDir), 0.0);
        // 本身纹理颜色 * 角度的“衰减” * 光的作用的“衰减” * 距离“衰减”
        vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) *
            diffuseStrength *
            light.diffuse *
            distanceAttenuation *
            flashLightAttenuation;
        
        vec3 reflectedLightDir = reflect(-normalizedFragmentLightPositionDirection, normalizedNormalDir);
        vec3 normalizedViewDir = normalize(viewPos - FragPos);
        float spec = pow(max(dot(reflectedLightDir, normalizedViewDir) ,0.0), material.shininess);
        vec3 specular =  texture(material.specular, TexCoords).rgb *
            spec *
            light.specular *
            distanceAttenuation *
            flashLightAttenuation;

        FragColor = vec4((ambient + diffuse + specular), 1.0);
    }
    else
    {
        // 否则只有环境光
        FragColor = vec4(vec3(texture(material.diffuse, TexCoords)) * light.ambient, 1.0);
    }
}
