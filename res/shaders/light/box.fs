#version 330 core
struct Material
{
    sampler2D diffuse;
    // vec3 specular;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
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
    float quadratic;
};

out vec4 FragColor;

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec3 Normal;
// 世界坐标系的坐标
in vec3 FragPos;
in vec2 TexCoords;

vec3 calcDirLight(DirLight light, vec3 normalDir, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normalDir, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normalDir, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 normalizedNormal = normalize(Normal);
    vec3 normalizedView = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(dirLight, normalizedNormal, normalizedView);

    for (int i = 0; i < NR_POINT_LIGHTS; i ++)
    {
        result += calcPointLight(pointLights[i], normalizedNormal, FragPos, normalizedView);
    }

    result += calcSpotLight(spotLight, normalizedNormal, FragPos, normalizedView);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normalDir, vec3 viewDir)
{
    vec3 diffuseTexture = texture(material.diffuse, TexCoords).rgb;
    vec3 normalizedLightDir = normalize(-light.direction);
    // 环境光
    vec3 ambient = light.ambient * diffuseTexture;

    // diffuse
    float diffuseDirectionAttenuation = max(dot(normalDir, normalizedLightDir), 0.0);
    vec3 diffuse = diffuseTexture *
        light.diffuse *
        diffuseDirectionAttenuation;
    
    vec3 reflectLightDir = reflect(-normalizedLightDir, normalDir);
    float specularDirectionAttenuation = pow(max(dot(reflectLightDir, viewDir), 0.0), material.shininess);
    vec3 specular = texture(material.specular, TexCoords).rgb *
        light.specular *
        specularDirectionAttenuation;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normalDir, vec3 fragPos, vec3 viewDir)
{
    vec3 diffuseTexture = texture(material.diffuse, TexCoords).rgb;
    vec3 originalLightDir = fragPos - light.position;
    vec3 normalizedLightDir = normalize(-originalLightDir);

    float distance = length(light.position - fragPos);
    float distanceAttenuation = 1 /
        (light.constant + distance * light.linear + distance * distance * light.quadratic);
    
    vec3 ambient = diffuseTexture * light.ambient * distanceAttenuation;

    float diffuseDirectionAttenuation = max(dot(normalDir, normalizedLightDir), 0.0);
    vec3 diffuse = diffuseTexture *
        light.diffuse *
        diffuseDirectionAttenuation *
        distanceAttenuation;
    
    vec3 reflectLightDir = reflect(-normalizedLightDir, normalDir);
    float specularDirectionAttenuation = pow(max(dot(reflectLightDir, viewDir), 0.0), material.shininess);
    vec3 specular = texture(material.specular, TexCoords).rgb *
        light.specular *
        specularDirectionAttenuation *
        distanceAttenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normalDir, vec3 fragPos, vec3 viewDir)
{
    // 着色点到光源的方向，射向光源
    vec3 normalizedFragmentLightPositionDirection =
        normalize(light.position - FragPos);
    // 射向光源的光方向
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
            light.quadratic * distance * distance
        );
        // 在flashLight这里不受距离衰减，否则远距离时，环境光在light内反而更小
        vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

        // 求diffuse
        vec3 normalizedNormalDir = normalize(normalDir);
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

        return ambient + diffuse + specular;
    }
    else
    {
        // 否则只有环境光
        return vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    }
}
