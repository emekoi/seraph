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
#include <SDL2/SDL.h>
#include "lib/sera/sera.h"
#include "util.h"
#include "m_buffer.h"
#include "m_graphics.h"

int m_graphics_maxFps = 60;

static int m_graphics_inited = 0;
static int m_graphics_fullscreen = 0;
static char *m_graphics_title = "";
static sr_Pixel m_graphics_clearColor;


static void resetVideoMode(void) {
  /* Reset video mode */
  int flags = (m_graphics_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL;

  SDL_CreateWindowAndRenderer(m_graphics_width,
    m_graphics_height, flags, &m_graphics_window, &m_graphics_renderer);
  if (!m_graphics_window || !m_graphics_renderer) CERROR("failed to create window or renderer");

  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  m_graphics_surface = SDL_CreateRGBSurface(0, m_graphics_width,
    m_graphics_height, 32, rmask, gmask, bmask, amask);

  /* Reset screen buffer */
  if (m_graphics_buffer) {
    m_graphics_buffer->pixels = (void *)m_graphics_surface->pixels;
    m_graphics_buffer->w = m_graphics_width;
    m_graphics_buffer->h = m_graphics_height;
    sr_setClip(m_graphics_buffer, RECT(m_graphics_buffer));
  }
}


void graphics_init(int w, int h, char *title, int fullscreen) {
  m_graphics_width = w;
  m_graphics_height = h;
  m_graphics_title = opt_string(title, "seraph");
  m_graphics_fullscreen = opt_number(fullscreen, 0);
  if (m_graphics_inited) {
    CERROR("graphics are already inited");
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    CERROR("could not init video");
  }
  /* Init SDL video */
  resetVideoMode();

  /* Init window title */
  SDL_SetWindowTitle(m_graphics_window, m_graphics_title);
  /* Create, store in registry and return main screen buffer */
  m_graphics_buffer = sr_newBufferShared(m_graphics_surface->pixels,
     m_graphics_width, m_graphics_height);
  /* Set state */
  m_graphics_inited = 1;
}


int graphics_setClearColor(sr_Pixel color) {
  m_graphics_clearColor = color;
  return 0;
}


sr_Pixel graphics_getClearColor(void) {
  return m_graphics_clearColor;
}


int graphics_clear(void) {
  sr_reset(m_graphics_buffer);
  sr_clear(m_graphics_buffer, m_graphics_clearColor);
  return 0;
}


int graphics_setSize(int width, int height) {
  m_graphics_width = opt_number(width, m_graphics_width);
  m_graphics_height = opt_number(height, m_graphics_height);
  resetVideoMode();
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
