/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void onInit() {
  graphics_setClearColor(sr_color(69, 0, 109));
}

void onDraw() {
  sr_drawBox(m_graphics_buffer, sr_color(255, 0, 0), bx, by, 64, 64);
  bx += 1;
  // by += 1;
}

void onQuit(void) {
  sr_destroyBuffer(m_graphics_buffer);
  SDL_DestroyRenderer(m_graphics_renderer);
  SDL_DestroyWindow(m_graphics_window);
  SDL_Quit();
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
  graphics_setMaxFps(60);
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

  // SDL_SetRenderDrawColor(m_graphics_renderer, 0, 0, 0, 255);
  // SDL_RenderClear(m_graphics_renderer);
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
