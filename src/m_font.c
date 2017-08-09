/**
 * Copyright (c) 2015 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "m_font.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "lib/stb_truetype.h"

/* ttf.c/h provides some higher level functionality around `stb_truetype` --
 * most importantly it provides a function which takes a string and a font and
 * returns an 8bit bitmap of the rendered string's text.
 *
 * For better results for our use case (writing each character straight to a
 * single buffer), stb_truetype.h has been slightly modified at the end of the
 * stbtt__rasterize_sorted_edges() function so that the character is added to
 * the buffer rather than overwriting whatever already exists in it; this is
 * beneficial in situations where character bounding boxes may overlap.
 */


ttf_Font *ttf_new(const void *data, int len) {
  ttf_Font *self = calloc(1, sizeof(*self));
  if (!self) {
    goto fail;
  }
  /* Copy font data */
  self->fontData = malloc(len);
  if (!self->fontData) {
    goto fail;
  }
  memcpy(self->fontData, data, len);
  /* Init font */
  if (!stbtt_InitFont(&self->font, self->fontData, 0)) {
    goto fail;
  }
  /* Init font size */
  ttf_ptsize(self, 14);
  return self;
fail:
  if (self) {
    ttf_destroy(self);
  }
  return NULL;
}


void ttf_destroy(ttf_Font *self) {
  free(self->fontData);
  free(self);
}


void ttf_ptsize(ttf_Font *self, float ptsize) {
  int ascent, descent, lineGap;
  self->ptsize = ptsize;
  self->scale = stbtt_ScaleForMappingEmToPixels(&self->font, self->ptsize);
  stbtt_GetFontVMetrics(&self->font, &ascent, &descent, &lineGap);
  self->baseline = ascent * self->scale + 1;
}


int ttf_height(ttf_Font *self) {
  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&self->font, &ascent, &descent, &lineGap);
  return ceil((ascent - descent + lineGap) * self->scale) + 1;
}


static const char *ttf_utf8toCodepoint(const char *p, unsigned *res) {
  unsigned x, mask, shift;
  switch (*p & 0xf0) {
    case 0xf0:  mask = 0x07;  shift = 18;   break;
    case 0xe0:  mask = 0x0f;  shift = 12;   break;
    case 0xc0:
    case 0xd0:  mask = 0x1f;  shift =  6;   break;
    default:
      *res = *p;
      return p + 1;
  }
  x = (*p & mask) << shift;
  do {
    /* Return early if we reach an unexpected NULL */
    if (*(++p) == '\0') {
      *res = x;
      return p;
    }
    shift -= 6;
    x |= (*p & 0x3f) << shift;
  } while (shift);
  *res = x;
  return p + 1;
}


static float ttf_charWidthf(ttf_Font *self, int c, int last) {
  int res = 0;
  int width, lsb;
  stbtt_GetCodepointHMetrics(&self->font, c, &width, &lsb);
  res = width;
  if (last) {
    int kerning = stbtt_GetCodepointKernAdvance(&self->font, last, c);
    res += kerning;
  }
  return res * self->scale;
}


int ttf_width(ttf_Font *self, const char *str) {
  float res = 0;
  int last = 0;
  const char *p = str;
  while (*p) {
    unsigned c;
    p = ttf_utf8toCodepoint(p, &c);
    res += ttf_charWidthf(self, c, last);
    last = c;
  }
  return ceil(res);
}


void *ttf_render(ttf_Font *self, const char *str, int *w, int *h) {
  *w = ttf_width(self, str);
  *h = ttf_height(self);
  void *pixels = calloc(1, *w * *h);
  if (!pixels) return NULL;
  const char *p = str;
  float xoffset = 0;
  float xfract = 0;
  int last = 0;
  while (*p) {
    /* Get unicode codepoint */
    unsigned c;
    p = ttf_utf8toCodepoint(p, &c);
    /* Get char placement coords */
    int x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBoxSubpixel(
      &self->font, c, self->scale, self->scale, xfract, 0,
      &x0, &y0, &x1, &y1);
    /* Work out position / max size */
    int x = xoffset + x0;
    int y = self->baseline + y0;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    /* Render char */
    stbtt_MakeCodepointBitmapSubpixel(
      &self->font,
      pixels + x + (y * *w),
      *w - x, *h - y, *w, self->scale, self->scale,
      xfract, 0, c);
    /* Next */
    xoffset += ttf_charWidthf(self, c, last);
    xfract = xoffset - (int) xoffset;
    last = c;
  }
  return pixels;
}


/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "util.h"
#include "m_font.h"
#include "lib/fs/fs.h"
#include "lib/sera/sera.h"

#define DEFAULT_FONTSIZE 14


static ttf_Font *newFont() {
  ttf_Font *font = zrealloc(NULL, sizeof(*font));
  return font;
}

static const char *loadFontFromMemory(
  ttf_Font *font, const void *data, int len, int ptsize
) {
  font = ttf_new(data, len);
  if (!font) return "could not load font";
  ttf_ptsize(font, ptsize);
  return NULL;
}


ttf_Font *font_fromFile(const char *filename, int fontsize) {
  fontsize = opt_number(fontsize, DEFAULT_FONTSIZE);
  ttf_Font *font = newFont();
  size_t len;
  void *data = fs_read(filename, &len);
  /* Load new font */
  if (!data) CERROR("could not open file '%s'", filename);
  const char *err = loadFontFromMemory(font, data, len, fontsize);
  free(data);
  if (err) CERROR("%s", err);
  return font;
}


ttf_Font *font_fromString(const char *data, int fontsize) {
  size_t len = strlen(data);
  fontsize = opt_number(fontsize, DEFAULT_FONTSIZE);
  ttf_Font *font = newFont();
  const char *err = loadFontFromMemory(font, data, len, fontsize);
  if (err) CERROR("%s", err);
  return font;
}


ttf_Font *font_fromEmbedded(int fontsize) {
  #include "default_ttf.h"
  fontsize = opt_number(fontsize, DEFAULT_FONTSIZE);
  ttf_Font *font = newFont();
  const char *err = loadFontFromMemory(font, default_ttf, sizeof(default_ttf),
                                       fontsize);
  if (err) CERROR("%s", err);
  return font;
}


sr_Buffer *font_render(ttf_Font *font, const char *str) {
  int w, h;
  if (!str || *str == '\0') str = " ";
  void *data = ttf_render(font, str, &w, &h);
  if (!data) CERROR("could not render text");
  /* Load bitmap and free intermediate 8bit bitmap */
  sr_Buffer *b = sr_newBuffer(w, h);
  if (!b) {
    free(data);
    CERROR("could not create buffer");
  }
  sr_loadPixels8(b, data, NULL);
  free(data);
  return b;
}


int sr_DrawText(sr_Buffer *b, ttf_Font *font, const char *str, int x, int y, sr_Transform *t) {
  sr_Buffer *txt = font_render(font, str);
  sr_drawBuffer(b, txt, x, y, NULL, t);
  return 0;
}


int font_getWidth(ttf_Font *font, const char *str) {
  return ttf_width(font, str);
}


int font_getHeight(ttf_Font *font) {
  return ttf_height(font);
}


int font_getSize(ttf_Font *font) {
  return font->ptsize;
}
