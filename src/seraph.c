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
  #define pxfmt SDL_PIXELFORMAT_RGBA32
#elif SR_MODE_ARGB
  #define pxfmt SDL_PIXELFORMAT_ARGB32
#elif SR_MODE_ABGR
  #define pxfmt SDL_PIXELFORMAT_ABGR32
#else
  #define pxfmt SDL_PIXELFORMAT_BGRA32
#endif

float bx = 0, by = 0;
Font *font;
sr_Buffer *hello;

void onInit() {
  hello = sr_BufferFile("hello_world.png");
  sr_drawBox(hello, sr_color(0, 140, 140), 0, 0, 128, 128);
  font = font_fromEmbedded(16);
  graphics_setClearColor(sr_color(69, 0, 109));
}

void onDraw() {
  /* Draw and reset the image buffer */
  sr_Transform t0 = sr_transform(0, 0, 0, 4, 4);
  sr_drawBuffer(m_graphics_buffer, hello, 0, 0, NULL, &t0);
  char buf[65]; sprintf(buf, "%d FPS", time_getFps());
  sr_drawText(m_graphics_buffer, font, sr_color(200, 200, 200), buf, 8, 6, NULL);
}

void onQuit(void) {
  sr_destroyBuffer(hello); font_gc(font);
  graphics_close();
  SDL_Quit(); fs_deinit();
}

int main(int argc, char **argv) {
  #if _WIN32
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  #endif
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
  graphics_init(width, height, "seraph", 0, 0);
  graphics_setMaxFps(60); fs_error(fs_mount("data"));
  onInit();
}

void __draw(void) {
  graphics_clear(); onDraw();
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
