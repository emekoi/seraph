#version 120

vec2 radialDistortion(vec2 coord) {
  vec2 cc = coord - 0.5;
  float dist = dot(cc, cc) * .2;
  return (coord + cc * (1.0 + dist) * dist);
}

float random(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  vec2 v1 = vec2(random(vec2(56.2, 0.72)), random(vec2(525.4, 35.34)));
  vec2 v2 = vec2(random(vec2(56.2, 0.72)), random(vec2(525.4, 35.34)));
  gl_FragColor = vec4(v1, v2);
}
