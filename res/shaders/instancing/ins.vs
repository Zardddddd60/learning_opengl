#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 offset;

out vec3 out_Color;

void main()
{
    gl_Position = vec4(offset + pos * (gl_InstanceID / 100.0), 0.0, 1.0);
    out_Color = color;
}
