/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_GRAPHICS_H
#define M_GRAPHICS_H

#include <SDL2/SDL.h>
#include "m_buffer.h"
#include "m_system.h"

SDL_Window *m_graphics_window;
SDL_Renderer *m_graphics_renderer;
SDL_Texture *m_graphics_texture;
int m_graphics_screenWidth;
int m_graphics_screenHeight;

Buffer *graphics_init(int width, int height, char *title, int fullscreen, int resizable, int borderless);
int graphics_clear(void);
sr_Pixel graphics_getClearColor(void);
int graphics_setClearColor(sr_Pixel color);
int graphics_setSize(int width, int height);
int graphics_setFullscreen(int b);
int graphics_getFullscreen(void);
int graphics_setMaxFps(int fps);
int graphics_getMaxFps(void);

#endif
