/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/sera/sera.h"
#include "m_buffer.h"
#include "util.h"
#include "lib/fs/fs.h"


Buffer *buffer_new(void) {
  Buffer *self = zrealloc(NULL, sizeof(*self));
  return self;
}


static sr_Pixel getColorArgs(int defzero, double r, double g, double b, double a) {
  float n = defzero ? 0. : 1.;
  double _r = (r ? r : n) * 256;
  double _g = (g ? g : n) * 256;
  double _b = (b ? b : n) * 256;
  double _a = (a ? a : n) * 256;
  return sr_pixel(_r, _g, _b, _a);
}


static void checkSubRect(sr_Buffer *b, sr_Rect *r) {
  if (r->x < 0 || r->y < 0 || r->x + r->w > b->w || r->y + r->h > b->h) {
    CERROR("sub rectangle out of bounds");
  }
}


static int loadBufferFromMemory(Buffer *self, const void *data, int len) {
  int w, h;
  void *pixels = stbi_load_from_memory(
    data, len, &w, &h, NULL, STBI_rgb_alpha);
  if (!pixels) {
    return -1;
  }
  self->buffer = sr_newBuffer(w, h);
  if (!self->buffer) {
    free(pixels);
    return -1;
  }
  sr_loadPixels(self->buffer, pixels, SR_FMT_RGBA);
  free(pixels);
  return 0;
}


Buffer *buffer_fromFile(const char *filename) {
  Buffer *self = buffer_new();
  size_t len;
  void *data = fs_read(filename, &len);
  if (!data) {
    CERROR("could not open file '%s'", filename);
  }
  int err = loadBufferFromMemory(self, data, len);
  free(data);
  if (err) {
    CERROR("could not load buffer");
  }
  return self;
}


Buffer *buffer_fromString(const char *str) {
  size_t len = strlen(str);
  Buffer *self = buffer_new();
  int err = loadBufferFromMemory(self, str, len);
  if (err) {
    CERROR("could not load buffer");
  }
  return self;
}


Buffer *buffer_fromBlank(double w, double h) {
  if (w <= 0) CERROR("expected width greater than 0");
  if (h <= 0) CERROR("expected height greater than 0");
  Buffer *self = buffer_new();
  self->buffer = sr_newBuffer(w, h);
  buffer_clear(self, sr_pixel(0, 0, 0, 0));
  if (!self->buffer) {
    CERROR("could not create buffer");
  }
  return self;
}


Buffer *buffer_clone(Buffer *self) {
  Buffer *b = buffer_new();
  b->buffer = sr_cloneBuffer(self->buffer);
  if (!b->buffer) {
    CERROR("could not clone buffer");
  }
  return b;
}


int buffer_gc(Buffer *self) {
  if (self->buffer) {               /* self->buffer may be NULL if  */
    sr_destroyBuffer(self->buffer); /* an error was raised in the   */
  }                                 /* constructor                  */
  return 0;
}


int buffer_getWidth(Buffer *self) {
  return self->buffer->w;
}

int buffer_getHeight(Buffer *self) {
  return self->buffer->h;
}


int buffer_setAlpha(Buffer *self, double alpha) {
  sr_setAlpha(self->buffer, opt_number(alpha, 1.) * 0xff);
  return 0;
}


int buffer_setBlend(Buffer *self, char *str) {
  str = opt_string(str, "alpha");
  int mode = 0;
  if      (!strcmp(str, "alpha"     )) mode = SR_BLEND_ALPHA;
  else if (!strcmp(str, "color"     )) mode = SR_BLEND_COLOR;
  else if (!strcmp(str, "add"       )) mode = SR_BLEND_ADD;
  else if (!strcmp(str, "subtract"  )) mode = SR_BLEND_SUBTRACT;
  else if (!strcmp(str, "multiply"  )) mode = SR_BLEND_MULTIPLY;
  else if (!strcmp(str, "lighten"   )) mode = SR_BLEND_LIGHTEN;
  else if (!strcmp(str, "darken"    )) mode = SR_BLEND_DARKEN;
  else if (!strcmp(str, "screen"    )) mode = SR_BLEND_SCREEN;
  else if (!strcmp(str, "difference")) mode = SR_BLEND_DIFFERENCE;
  else CERROR("bad blend mode");
  sr_setBlend(self->buffer, mode);
  return 0;
}


int buffer_setColor(Buffer *self, double r, double g, double b, double a) {
  sr_setColor(self->buffer, getColorArgs(0, r, g, b, a));
  return 0;
}


int buffer_setClip(Buffer *self, sr_Rect clip) {
  sr_setClip(self->buffer, clip);
  return 0;
}


int buffer_reset(Buffer *self) {
  sr_reset(self->buffer);
  return 0;
}


int buffer_clear(Buffer *self, sr_Pixel color) {
  sr_clear(self->buffer, color);
  return 0;
}


sr_Pixel buffer_getPixel(Buffer *self, double x, double y) {
  sr_Pixel px = sr_getPixel(self->buffer, x, y);
  px.rgba.r *= 0.00390625; /* div 256. */
  px.rgba.g *= 0.00390625;
  px.rgba.b *= 0.00390625;
  px.rgba.a *= 0.00390625;
  return px;
}


int buffer_setPixel(Buffer *self, double x, double y, double r, double g, double b, double a) {
  sr_setPixel(self->buffer, getColorArgs(0, r, g, b, a), x, y);
  return 0;
}


int buffer_copyPixels(Buffer *self, Buffer *src, double x, double y, sr_Rect sub, double sx, double sy) {
  int hasSub = 0;
  if (sub.x) {
    hasSub = 1;
    checkSubRect(src->buffer, &sub);
  }
  sy = opt_number(sy, sx);
  sr_copyPixels(self->buffer, src->buffer, x, y,
                 hasSub ? &sub : NULL, sx, sy);
  return 0;
}


int buffer_noise(Buffer *self, double seed, double low, double high, double grey) {
  seed = opt_number(seed, rand());
  low  = opt_number(low, 0) * 256;
  high = opt_number(high, 1) * 256;
  grey = opt_number(grey, 0);
  sr_noise(self->buffer, seed, low, high, grey);
  return 0;
}


int buffer_floodFill(Buffer *self, double x, double y, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g, b, a);
  sr_floodFill(self->buffer, px, x, y);
  return 0;
}


int buffer_drawPixel(Buffer *self, double x, double y, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g, b, a);
  sr_drawPixel(self->buffer, px, x, y);
  return 0;
}


int buffer_drawLine(Buffer *self, double x1, double y1, double x2, double y2, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g, b, a);
  sr_drawLine(self->buffer, px, x1, y1, x2, y2);
  return 0;
}

int buffer_drawRect(Buffer *self, double x, double y, double w, double h, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g, b, a);
  sr_drawRect(self->buffer, px, x, y, w, h);
  return 0;
}


int buffer_drawBox(Buffer *self, double x, double y, double w, double h, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g, b, a);
  sr_drawBox(self->buffer, px, x, y, w, h);
  return 0;
}


int buffer_drawCircle(Buffer *self, double x, double y, double rad, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g , b, a);
  sr_drawCircle(self->buffer, px, x, y, rad);
  return 0;
}


int buffer_drawRing(Buffer *self, double x, double y, double rad, double r, double g, double b, double a) {
  sr_Pixel px = getColorArgs(0, r, g , b, a);
  sr_drawRing(self->buffer, px, x, y, rad);
  return 0;
}


int buffer_drawBuffer(Buffer *self, Buffer *src, double x, double y, sr_Rect sub, double rot, double sx, double sy, double ox, double oy) {
  int hasSub = 0;
  sr_Transform t;
  if (sub.x) {
    hasSub = 1;
    checkSubRect(src->buffer, &sub);
  }
  t.r  = rot;
  t.sx = sx;
  t.sy = sy;
  t.ox = ox;
  t.oy = oy;
  sr_drawBuffer(self->buffer, src->buffer, x, y, hasSub ? &sub : NULL, &t);
  return 0;
}
