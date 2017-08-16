#version 120

uniform vec2 position;

vec2 radialDistortion(vec2 coord) {
  vec2 cc = coord - 0.5;
  float dist = dot(cc, cc) * .2;
  return (coord + cc * (1.0 + dist) * dist);
}

void main() {
  gl_Position = vec4(radialDistortion(position), 0.0, 1.0);
}
