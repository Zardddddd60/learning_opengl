#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// out vec2 TexCoords;
// out vec3 FragPos;
// out vec3 Normal;

out VS_OUT {
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // FragPos = vec3(model * vec4(aPos, 1.0));
    // TexCoords = aTexCoords;
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 传到fs里贴图用
    vs_out.TexCoords = aTexCoords;
}