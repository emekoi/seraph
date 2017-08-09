/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chadar.h"

void onInit() {
  graphics_setClearColor(sr_pixel(69, 0, 109, 255));
}

void onDraw() {
  sr_drawBox(m_graphics_buffer, sr_color(255, 34, 25), 24, 23, 64, 64);
}

void onQuit(void) {
  sr_destroyBuffer(m_graphics_buffer);
  SDL_FreeSurface(m_graphics_surface);
  SDL_DestroyRenderer(m_graphics_renderer);
  SDL_DestroyWindow(m_graphics_window);
  SDL_Quit();
}

int main(int argc, char **argv) {
  atexit(onQuit);
  __init();

  for (;;) {
    time_step();
    __draw();
  }

  return 0;
}

void __init(void) {
  int width = 512, height = 512;
  graphics_init(width, height, "seraph", 0);
  graphics_setMaxFps(60);
  onInit();
}

void __draw(void) {
  graphics_clear();
  onDraw();
  static double last = 0;

  // SDL_UpdateTexture(m_graphics_texture, NULL, m_graphics_surface->pixels, m_graphics_width * sizeof(Uint32));
  // SDL_SetRenderDrawColor(m_graphics_renderer, 0, 0, 0, 255);
  SDL_Texture *m_graphics_texture = SDL_CreateTextureFromSurface(m_graphics_renderer, m_graphics_surface);
  // SDL_RenderCopy(m_graphics_renderer, m_graphics_texture, NULL, NULL);
  SDL_RenderClear(m_graphics_renderer);
  SDL_RenderPresent(m_graphics_renderer);

  /* Wait for next frame */
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
