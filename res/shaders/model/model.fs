#version 330 core
struct Material
{
    sampler2D texture_diffuse_1;
    sampler2D texture_specular_1;
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
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[2];
uniform SpotLight spotLight;
uniform vec3 viewPos;

vec3 clacDirLight(DirLight light, vec3 normalizedNormal, vec3 normalizedViewDir);
vec3 clacPointLight(PointLight light, vec3 normalizedNormal, vec3 normalizedViewDir);
vec3 clacSpotLight(SpotLight light, vec3 normalizedNormal, vec3 normalizedViewDir);

void main()
{    
    vec3 normalizedNormal = normalize(Normal);
    vec3 normalizedViewDir = normalize(viewPos - FragPos);

    vec3 result = clacDirLight(dirLight, normalizedNormal, normalizedViewDir);

    for (int i = 0; i < 2; i ++)
    {
        result += clacPointLight(pointLights[i], normalizedNormal, normalizedViewDir);
    }

    result += clacSpotLight(spotLight, normalizedNormal, normalizedViewDir);

    FragColor = vec4(result, 1.0);
}

// 不用考虑距离上的问题
vec3 clacDirLight(DirLight light, vec3 normalizedNormal, vec3 normalizedViewDir)
{
    vec3 diffuseTexture = texture(material.texture_diffuse_1, TexCoords).rgb;
    vec3 ambient = diffuseTexture * light.ambient;

    vec3 normalizedPos2LightDir = normalize(-light.direction);
    float directionAttenuation = max(dot(normalizedPos2LightDir, normalizedNormal), 0.0);
    vec3 diffuse = diffuseTexture * light.diffuse * directionAttenuation;

    vec3 reflectedLightDir = -normalizedPos2LightDir;
    vec3 specularTexture = texture(material.texture_specular_1, TexCoords).rgb;
    directionAttenuation = pow(max(dot(normalizedViewDir, reflectedLightDir), 1.0), material.shininess);
    vec3 specular = specularTexture * light.specular * directionAttenuation;

    return ambient + diffuse + specular;
}

vec3 clacPointLight(PointLight light, vec3 normalizedNormal, vec3 normalizedViewDir)
{
    vec3 diffuseTexture = texture(material.texture_diffuse_1, TexCoords).rgb;
    vec3 normalizedPos2LightDir = normalize(light.position - FragPos);
    float distance_ = length(light.position - FragPos);
    float distanceAttenuation = 1 / (
        light.constant + distance_ * light.linear + distance_ * distance_ * light.quadratic
    );

    vec3 ambient = diffuseTexture * light.ambient * distanceAttenuation;

    float directionAttenuation = max(dot(normalizedPos2LightDir, normalizedNormal), 0.0);
    vec3 diffuse = diffuseTexture * light.diffuse * directionAttenuation * distanceAttenuation;

    vec3 reflectedLightDir = -normalizedPos2LightDir;
    vec3 specularTexture = texture(material.texture_specular_1, TexCoords).rgb;
    directionAttenuation = pow(max(dot(normalizedViewDir, reflectedLightDir), 1.0), material.shininess);
    vec3 specular = specularTexture * light.specular * directionAttenuation * distanceAttenuation;

    return ambient + diffuse + specular;
}

vec3 clacSpotLight(SpotLight light, vec3 normalizedNormal, vec3 normalizedViewDir)
{

    vec3 diffuseTexture = texture(material.texture_diffuse_1, TexCoords).rgb;
    float distance_ = length(light.position - FragPos);
    float distanceAttenuation = 1 / (
        light.constant + distance_ * light.linear + distance_ * distance_ * light.quadratic
    );
    vec3 ambient = diffuseTexture * light.ambient * distanceAttenuation;
    
    vec3 normalized2LightPosDir = normalize(light.position - FragPos);
    vec3 normalized2LighDir = normalize(-light.direction);
    float theta = max(0.0, dot(normalized2LightPosDir, normalized2LighDir));
    float epsilon = light.cutOff - light.outerCutOff;
    if (theta > light.outerCutOff)
    {
        float spotLightAttenuation = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        vec3 normalizedPos2LightDir = normalize(light.position - FragPos);

        float directionAttenuation = max(dot(normalizedPos2LightDir, normalizedNormal), 0.0);
        vec3 diffuse =
            diffuseTexture *
            light.diffuse *
            directionAttenuation *
            distanceAttenuation *
            spotLightAttenuation;

        vec3 reflectedLightDir = -normalizedPos2LightDir;
        vec3 specularTexture = texture(material.texture_specular_1, TexCoords).rgb;
        directionAttenuation = pow(max(dot(normalizedViewDir, reflectedLightDir), 1.0), material.shininess);
        vec3 specular =
            specularTexture *
            light.specular *
            directionAttenuation *
            distanceAttenuation *
            spotLightAttenuation;

        return ambient + diffuse + specular;
    }
    else
    {
        return ambient;
    }

}
