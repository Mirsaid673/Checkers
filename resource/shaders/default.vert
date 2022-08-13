#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 uv;

uniform vec2 pos;
uniform mat4 v;
uniform mat4 p;
void main() 
{
    gl_Position = p * v * vec4(aPos + pos, 0.0f, 1.0f);
    
    uv = aTexCoord;
}