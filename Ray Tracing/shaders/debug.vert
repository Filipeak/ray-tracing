#version 430 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_VP;

void main()
{
    gl_Position = u_VP * vec4(aPos, 1.0);
}