#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

float far = 100.0;
float near = 1.0;

float linearizeDepth(float depth)
{
    // [0, 1] => [-1, 1]
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));	
}

void main()
{
    // FragColor = texture(texture1, TexCoord);
    // 很远的时候才是白的
    float depth = linearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth) ,1.0);
}
