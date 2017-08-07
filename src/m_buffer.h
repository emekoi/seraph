/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_BUFFER_H
#define M_BUFFER_H

#include "lib/sera/sera.h"
// #include "m_font.h"

typedef struct {
  sr_Buffer *buffer;
} Buffer;

Buffer *buffer_new(void);
Buffer *buffer_fromFile(const char *filename);
Buffer *buffer_fromString(const char *str);
Buffer *buffer_fromBlank(double w, double h);
Buffer *buffer_clone(Buffer *self);
int buffer_gc(Buffer *self);
int buffer_getWidth(Buffer *self);
int buffer_getHeight(Buffer *self);
int buffer_setAlpha(Buffer *self, double alpha);
int buffer_setBlend(Buffer *self, char *str);
int buffer_setColor(Buffer *self, double r, double g, double b, double a);
int buffer_setClip(Buffer *self, sr_Rect clip);
int buffer_reset(Buffer *self);
int buffer_clear(Buffer *self, sr_Pixel color);
sr_Pixel buffer_getPixel(Buffer *self, double x, double y);
int buffer_setPixel(Buffer *self, double x, double y, double r, double g, double b, double a);
int buffer_copyPixels(Buffer *self, Buffer *src, double x, double y, sr_Rect sub, double sx, double sy);
int buffer_noise(Buffer *self, double seed, double low, double high, double grey);
int buffer_floodFill(Buffer *self, double x, double y, double r, double g, double b, double a);
int buffer_drawPixel(Buffer *self, double x, double y, double r, double g, double b, double a);
int buffer_drawLine(Buffer *self, double x1, double y1, double x2, double y2, double r, double g, double b, double a);
int buffer_drawRect(Buffer *self, double x, double y, double w, double h, double r, double g, double b, double a);
int buffer_drawBox(Buffer *self, double x, double y, double w, double h, double r, double g, double b, double a);
int buffer_drawCircle(Buffer *self, double x, double y, double rad, double r, double g, double b, double a);
int buffer_drawRing(Buffer *self, double x, double y, double rad, double r, double g, double b, double a);
int buffer_drawBuffer(Buffer *self, Buffer *src, double x, double y, sr_Rect sub, double rot, double sx, double sy, double ox, double oy);

#endif
