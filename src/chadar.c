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


extern int m_graphics_maxFps;
Font *font;
Buffer *SCREEN;
Buffer *hello;


void onInit() {
  hello = buffer_fromFile("hello_world.png");
  font = font_fromEmbedded(16);
  graphics_setClearColor(sr_pixel(69, 0, 109, 255));
}

void onUpdate(double dt) {
  if (keyboard_wasPressed("escape")) {
    exit(EXIT_SUCCESS);
  }
}

void onDraw() {
  buffer_reset(hello);
  buffer_copyPixels(SCREEN, hello, 0, 0, RECT(hello), 4, 4);
  char buf[64];sprintf(buf, "%d", time_getFps());
  buffer_drawText(SCREEN, font, buf, 4, 4, 2, 2);
}

void onKeyDown(char *key, char *chr) {}

void onKeyUp(char *key) {}

void onQuit(void) {
  #ifndef __APPLE__
    SDL_Quit();
  #endif
    buffer_gc(hello);
    buffer_gc(SCREEN);
    font_gc(font);
    fs_unmount("data");
    fs_deinit();
    __keyboard_close();
}


int main(int argc, char **argv) {
  atexit(onQuit);
  __init();
  /* Do main loop */
  for (;;) {
    onStepMain();
  }

  return 0;
}


void onStepMain(void) {
  system_poll();
  time_step();
  onUpdate(time_getDelta());
  __draw();
  __keyboard_reset();
}


void __init(void) {
  int width = 512, height = 512;
  int fullscreen = 0, resizable = 0, borderless = 0;
  char *title = "seraph";
  SCREEN = graphics_init(width, height, title, fullscreen, resizable, borderless);
  graphics_setMaxFps(60);
  fs_error(fs_mount("data"));
  fs_error(fs_setWritePath("data"));
  __keyboard_open();
  onInit();
}


void __draw(void) {
  graphics_clear();
  onDraw();
  static double last = 0;

  sr_Buffer *buffer = SCREEN->buffer;
  SDL_UpdateTexture(m_graphics_texture, NULL, buffer->pixels, m_graphics_screenWidth * sizeof(Uint32));
  // SDL_SetRenderDrawColor(m_graphics_renderer, 0, 0, 0, 255);
  SDL_RenderCopy(m_graphics_renderer, m_graphics_texture, NULL, NULL);
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
