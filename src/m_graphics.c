/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <SDL/SDL.h>
#include "lib/sera/sera.h"
#include "util.h"
#include "m_buffer.h"
#include "m_graphics.h"

int m_graphics_maxFps = 60.;

static int m_graphics_inited = 0;
static int m_graphics_screenWidth = 0;
static int m_graphics_screenHeight = 0;
static int m_graphics_fullscreen = 0;
static int m_graphics_resizable = 0;
static int m_graphics_borderless = 0;
static char *m_graphics_title = "";
static sr_Pixel m_graphics_clearColor;
static Buffer *m_graphics_screen;


static void resetVideoMode(void) {
  /* Reset video mode */
  int flags = (m_graphics_fullscreen ? SDL_FULLSCREEN : 0) |
              (m_graphics_resizable  ? SDL_RESIZABLE : 0)  |
              (m_graphics_borderless ? SDL_NOFRAME : 0) | SDL_DOUBLEBUF;
  if (SDL_SetVideoMode(m_graphics_screenWidth, m_graphics_screenHeight, 32, flags) == NULL) {
    CERROR("could not set video mode");
  }
  /* Reset screen buffer */
  if (m_graphics_screen) {
    sr_Buffer *b = m_graphics_screen->buffer;
    b->pixels = (void*) SDL_GetVideoSurface()->pixels;
    b->w = m_graphics_screenWidth;
    b->h = m_graphics_screenHeight;
    sr_setClip(b, sr_rect(0, 0, b->w, b->h));
  }
}


Buffer *graphics_init(int w, int h, char *title, int fullscreen, int resizable, int borderless) {
  m_graphics_screenWidth = w;
  m_graphics_screenHeight = h;
  m_graphics_title = opt_string(title, "chadar");
  m_graphics_fullscreen = fullscreen;
  m_graphics_resizable = resizable;
  m_graphics_borderless = borderless;
  if (m_graphics_inited) {
    CERROR("graphics are already inited");
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    CERROR("could not init video");
  }
  /* Init SDL video */
  resetVideoMode();

  /* Required to get the associated character when a key is pressed. This has
   * to be enabled *after* SDL video is set up */
  SDL_EnableUNICODE(1);
  /* Init window title */
  SDL_WM_SetCaption(m_graphics_title, m_graphics_title);
  /* Create, store in registry and return main screen buffer */
  m_graphics_screen = buffer_new();
  m_graphics_screen->buffer = sr_newBufferShared(
    SDL_GetVideoSurface()->pixels, m_graphics_screenWidth, m_graphics_screenHeight);
  /* Set state */
  m_graphics_inited = 1;
  return m_graphics_screen;
}


int graphics_setClearColor(sr_Pixel color) {
  m_graphics_clearColor = color;
  return 0;
}


sr_Pixel graphics_getClearColor(void) {
  return m_graphics_clearColor;
}


int graphics_clear(void) {
  buffer_reset(m_graphics_screen);
  buffer_clear(m_graphics_screen, m_graphics_clearColor);
  return 0;
}


int graphics_setSize(int width, int height) {
  width = opt_number(width, m_graphics_screenWidth);
  height = opt_number(height, m_graphics_screenHeight);
  /* Reset video mode and set new screen size*/
  int flags = (m_graphics_fullscreen ? SDL_FULLSCREEN : 0) |
              (m_graphics_resizable  ? SDL_RESIZABLE : 0)  |
              (m_graphics_borderless ? SDL_NOFRAME : 0) | SDL_DOUBLEBUF;
  if (SDL_SetVideoMode(width, height, 32, flags) == NULL) {
    CERROR("could not set resize screen");
  }
  /* Reset screen buffer */
  if (m_graphics_screen) {
    sr_Buffer *b = m_graphics_screen->buffer;
    b->pixels = (void*) SDL_GetVideoSurface()->pixels;
    b->w = width;
    b->h = height;
    sr_setClip(b, sr_rect(0, 0, b->w, b->h));
  }
  return 0;
}

int graphics_setFullscreen(int b) {
  m_graphics_fullscreen = opt_number(b, 0);
  resetVideoMode();
  return 0;
}

int graphics_getFullscreen(void) {
  return m_graphics_fullscreen;
}

int graphics_setMaxFps(int fps) {
  m_graphics_maxFps = opt_number(fps, 60);
  return 0;
}

int graphics_getMaxFps(void) {
  return m_graphics_maxFps;
}


void __graphics_on_event(event_t *e) {
  switch (e->type) {
    case RESIZE:
      graphics_setSize(e->resize.width, e->resize.height);
      break;
  }
}
