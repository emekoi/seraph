#version 120

#define Image sampler2D
#define extern uniform
#define Texel texture2D

attribute vec2 tex_coord_in;
attribute vec2 position_in;
attribute vec3 color_in;

varying vec3 color_out;
varying vec2 tex_coord_out;

vec2 radialDistortion(vec2 coord) {
  // vec2 cc = coord - 0.5;
  // float dist = dot(cc, cc) * .2;
  // return (coord + cc * (1.0 + dist) * dist);
  return coord;
}

void main() {
  color_out = color_in;
  gl_Position = vec4(radialDistortion(position_in), 1.0, 1.0);
  tex_coord_out = tex_coord_in;
}
