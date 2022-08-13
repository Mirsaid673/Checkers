#version 330 core

in vec2 uv;
uniform sampler2D tex;

out vec4 Frag;
void main(){
  Frag = texture(tex, uv);
}