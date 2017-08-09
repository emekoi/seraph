/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/fs/fs.h"
#include "seraph.h"

#if SR_MODE_RGBA
Uint32 pxfmt = SDL_PIXELFORMAT_RGBA32;
#elif SR_MODE_ARGB
Uint32 pxfmt = SDL_PIXELFORMAT_ARGB32;
#elif SR_MODE_ABGR
Uint32 pxfmt = SDL_PIXELFORMAT_ABGR32;
#else
Uint32 pxfmt = SDL_PIXELFORMAT_BGRA32;
#endif

float bx = 0, by = 0;
ttf_Font *font;
sr_Buffer *hello;

void onInit() {
  // hello = sr_BufferBlank(512, 512);
  hello = sr_BufferFile("hello_world.png");
  font = font_fromEmbedded(16);
  graphics_setClearColor(sr_color(69, 0, 109));
}

void onDraw() {
  sr_Rect r = RECT(hello);
  sr_Transform t = sr_transform(0, 0, 0, 0, 0);
  sr_drawBox(hello, sr_color(3, 43, 56), 0, 4, 230, 230);
  sr_copyPixels(m_graphics_buffer, hello, 0, 0, &r, 4, 4);
  char buf[64]; sprintf(buf, "%d", time_getFps());
  sr_DrawText(m_graphics_buffer, font, buf, 4, 4, &t);
}

void onQuit(void) {
  sr_destroyBuffer(m_graphics_buffer);
  sr_destroyBuffer(hello); // ttf_destroy(font);
  SDL_DestroyRenderer(m_graphics_renderer);
  SDL_DestroyWindow(m_graphics_window);
  SDL_Quit(); fs_deinit();
}

int main(int argc, char **argv) {
  atexit(onQuit);
  __init();

  for (;;) {
    time_step();
    system_poll();
    __draw();
  }

  return 0;
}

void __init(void) {
  int width = 512, height = 512;
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  graphics_init(width, height, "seraph", 0, 1);
  graphics_setMaxFps(60); fs_error(fs_mount("data"));
  onInit();
}

void __draw(void) {
  graphics_clear();
  onDraw();

  sr_Buffer *b = m_graphics_buffer;
  int w = sr_BufferWidth(b), h = sr_BufferHeight(b);
  int depth = 32, pitch = 4 * w;
  SDL_Surface* m_graphics_surface = SDL_CreateRGBSurfaceWithFormatFrom((void *)b->pixels, w, h, depth, pitch, pxfmt);
  if (!m_graphics_surface) CERROR("failed to create surface: %s", SDL_GetError());

  SDL_Texture *m_graphics_texture = SDL_CreateTextureFromSurface(m_graphics_renderer, m_graphics_surface);
  if (!m_graphics_texture) CERROR("failed to create texture: %s", SDL_GetError());
  SDL_FreeSurface(m_graphics_surface);

  SDL_RenderCopy(m_graphics_renderer, m_graphics_texture, NULL, NULL);
  SDL_RenderPresent(m_graphics_renderer);
  SDL_DestroyTexture(m_graphics_texture);

  /* Wait for next frame */
  static double last = 0;
  double step = (1. / m_graphics_maxFps);
  double now = SDL_GetTicks() / 1000.;
  double wait = step - (now - last);
  last += step;
  if (wait > 0) {
    SDL_Delay(wait * 1000.);
  } else {
    last = now;
  }
}