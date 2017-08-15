/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_GRAPHICS_H
#define M_GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "lib/sera/sera.h"

SDL_Window *m_graphics_window;
SDL_Renderer *m_graphics_renderer;
SDL_GLContext m_graphics_context;
sr_Buffer *m_graphics_buffer;

int m_graphics_width;
int m_graphics_height;
int m_graphics_maxFps;

void graphics_init(int w, int h, char *title, int fullscreen, int resizable);
void graphics_close(void);
int graphics_clear(void);
int graphics_setSize(int width, int height);
sr_Pixel graphics_getClearColor(void);
int graphics_setClearColor(sr_Pixel color);
int graphics_setFullscreen(int f);
int graphics_getFullscreen(void);
int graphics_setMaxFps(int fps);
int graphics_getMaxFps(void);

#endif
