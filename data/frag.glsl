#version 120

varying vec3 Color;
// uniform vec3 tri_color;

void main() {
  gl_FragColor = vec4(Color, 1.0);
}