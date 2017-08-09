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

ttf_Font *font_fromFile(const char *filename, int fontsize);
ttf_Font *font_fromString(const char *data, int fontsize);
ttf_Font *font_fromEmbedded(int fontsize);
sr_Buffer *font_render(ttf_Font *f, const char *str);
int sr_DrawText(sr_Buffer *b, ttf_Font *font, const char *str, int x, int y, sr_Transform *t);
int font_getWidth(ttf_Font *f, const char *str);
int font_getHeight(ttf_Font *f);
int font_getSize(ttf_Font *f);

#endif
