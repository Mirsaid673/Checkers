#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

//out vec3 color;
out vec3 color;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
void main() 
{
    gl_Position = p * v * m * vec4(aPos, 0.0f, 1.0f);
    //color = aColor;
    color = aColor;
}