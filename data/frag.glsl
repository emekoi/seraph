#version 120

#define Image sampler2D
#define extern uniform
#define Texel texture2D

varying vec3 color_out;
varying vec2 tex_coord_out;

extern Image tex;

void main() {
  gl_FragColor = Texel(tex, tex_coord_out) *  vec4(color_out, 1.0);
  gl_FragColor = Texel(tex, tex_coord_out);
}