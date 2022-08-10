#version 330 core

//in vec3 color;
in vec3 color;

out vec4 Frag;
void main(){
  Frag = vec4(color, 1);
}