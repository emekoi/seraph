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
#include "glew/glew.h"
// #include <SDL2/SDL_opengl.h>

#include "sera/sera.h"
#include "util.h"
#include "m_graphics.h"

int m_graphics_maxFps = 60;

static int m_graphics_inited = 0;
static int m_graphics_fullscreen = 0;
static int m_graphics_resizable = 0;
static char *m_graphics_title = "";
static sr_Pixel m_graphics_clearColor;


static void resetVideoMode(void) {
  /* Reset video mode */
  int flags = (m_graphics_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) |
              (m_graphics_resizable  ? SDL_WINDOW_RESIZABLE : 0);

  m_graphics_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_graphics_width, m_graphics_height, (flags | SDL_WINDOW_OPENGL));
  if (!m_graphics_window) CERROR("failed to create window");

  m_graphics_context = SDL_GL_CreateContext(m_graphics_window);
  // if (!m_graphics_context) TRACE("failed to create context: %s", SDL_GetError());

  m_graphics_renderer = SDL_CreateRenderer(m_graphics_window, -1, SDL_RENDERER_ACCELERATED);
  if (!m_graphics_renderer) CERROR("failed to create renderer");

  /* Reset screen buffer */
  sr_Buffer *buf = m_graphics_buffer;
  if (buf) {
    buf->w = m_graphics_width;
    buf->h = m_graphics_height;
    buf->pixels = realloc(buf->pixels, buf->w * buf->h * sizeof(*buf->pixels));
    sr_setClip(buf, RECT(buf));
  }
}


void graphics_init(int w, int h, char *title, int fullscreen, int resizable) {
  m_graphics_width = w; m_graphics_height = h;
  m_graphics_title = (title ? title : "seraph");
  m_graphics_fullscreen = (fullscreen ? fullscreen : 0);
  m_graphics_resizable = (resizable ? resizable : 0);
  if (m_graphics_inited) CERROR("graphics are already inited");

  /* Init SDL video subsystem*/
  if (SDL_Init(SDL_INIT_VIDEO) != 0) CERROR("could not init video");

  /* Setup OpenGL */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  resetVideoMode();

  /* Init GLEW */
  glewExperimental = GL_TRUE;
  glewInit(); GLuint vertexbuf;
  glGenBuffers(1, &vertexbuf);
  if (!vertexbuf) CERROR("failed to init GLEW");

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  TRACE("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  /* Set window title */
  SDL_SetWindowTitle(m_graphics_window, m_graphics_title);

  /* Create main screen buffer */
  m_graphics_buffer = sr_newBufferBlank(m_graphics_width, m_graphics_height);

  /* Set state */
  m_graphics_inited = 1;
}

void graphics_close(void) {
  sr_destroyBuffer(m_graphics_buffer);
  SDL_GL_DeleteContext(m_graphics_context);
  SDL_DestroyRenderer(m_graphics_renderer);
  SDL_DestroyWindow(m_graphics_window);
}


int graphics_setClearColor(sr_Pixel color) {
  m_graphics_clearColor = color;
  return 0;
}


sr_Pixel graphics_getClearColor(void) {
  return m_graphics_clearColor;
}


int graphics_clear(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  sr_clear(m_graphics_buffer, m_graphics_clearColor);
  sr_reset(m_graphics_buffer);
  return 0;
}


int graphics_setSize(int width, int height) {
  m_graphics_width = (width ? width : m_graphics_width);
  m_graphics_height = (height ? height : m_graphics_height);
  sr_Buffer *b = m_graphics_buffer;
  if (b) {
    b->w = m_graphics_width; b->h = m_graphics_height; sr_setClip(b, RECT(b));
    b->pixels = realloc(b->pixels, b->w * b->h * sizeof(*b->pixels));
  }
  return 0;
}

int graphics_setFullscreen(int f) {
  m_graphics_fullscreen = f;
  SDL_SetWindowFullscreen(m_graphics_window, f ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
  return 0;
}

int graphics_getFullscreen(void) {
  return m_graphics_fullscreen;
}

int graphics_setMaxFps(int fps) {
  m_graphics_maxFps = (fps ? fps : m_graphics_maxFps);
  return 0;
}

int graphics_getMaxFps(void) {
  return m_graphics_maxFps;
}
