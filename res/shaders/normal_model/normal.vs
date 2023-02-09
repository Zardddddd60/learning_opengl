#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;

out VS_OUT {
    vec3 Normal;
} vs_out;

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0);
    // vs_out.Normal = normalize(vec3(transpose(inverse(view * model)) * vec4(aNormal, 1.0)));
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.Normal = vec3(vec4(normalMatrix * aNormal, 0.0)); 
}
