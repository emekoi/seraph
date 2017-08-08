/**
 * Copyright (c) 2015 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_FONT_H
#define M_FONT_H

#include "m_buffer.h"
#include "lib/stb_truetype.h"

typedef struct {
  stbtt_fontinfo font;
  void *fontData;
  float ptsize;
  float scale;
  int baseline;
} ttf_Font;


ttf_Font *ttf_new(const void *data, int len);
void ttf_destroy(ttf_Font *self);
void ttf_ptsize(ttf_Font *self, float ptsize);
int ttf_height(ttf_Font *self);
int ttf_width(ttf_Font *self, const char *str);
void *ttf_render(ttf_Font *self, const char *str, int *w, int *h);

typedef struct {
  ttf_Font *font;
} Font;

Font *font_fromFile(const char *filename, int fontsize);
Font *font_fromString(const char *data, int fontsize);
Font *font_fromEmbedded(int fontsize);
int font_gc(Font *self);
Buffer *font_render(Font *self, const char *str);
int buffer_drawText(Buffer *self, Font *font, const char *str, double x, double y, double sx, double sy);
int font_getWidth(Font *self, const char *str);
int font_getHeight(Font *self);
int font_getSize(Font *self);

#endif
