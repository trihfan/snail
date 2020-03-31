#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 view;

void main()
{
   gl_Position = view * vec4(position, 1.f);
}
