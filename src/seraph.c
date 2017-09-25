/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs/fs.h"
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
Shader *shader;

void onInit() {
  hello = sr_newBufferFile("hello_world.png");
  sr_drawBox(hello, sr_color(0, 140, 140), 0, 0, 128, 128);
  font = font_fromEmbedded(16);
  graphics_setClearColor(sr_color(69, 0, 109));


float vertices[] = {
   0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
   0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
};

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo); // Generate 1 buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  shader = shader_fromFile("vert.glsl", "frag.glsl");
  shader_use(shader);

  shader_setAttribute(shader, "position", 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
  shader_setAttribute(shader, "color", 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
}

void onDraw() {
  /* Draw and reset the image buffer */
  // sr_Transform t0 = sr_transform(0, 0, 0, 4, 4);
  // sr_drawBuffer(m_graphics_buffer, hello, 0, 0, NULL, &t0);
  // char buf[65]; sprintf(buf, "%d FPS", time_getFps());
  // sr_drawText(m_graphics_buffer, font, sr_color(200, 200, 200), buf, 8, 6, NULL);
  
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void onQuit(void) {
  sr_destroyBuffer(hello); 
  font_destroy(font);
  graphics_close();
  shader_destroy(shader);
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
  // int w = b->w, h = b->h;
  // int depth = 32, pitch = 4 * w;
  // SDL_Surface* m_graphics_surface = SDL_CreateRGBSurfaceWithFormatFrom((void *)b->pixels, w, h, depth, pitch, pxfmt);
  // if (!m_graphics_surface) CERROR("failed to create surface: %s", SDL_GetError());

  // SDL_Texture *m_graphics_texture = SDL_CreateTextureFromSurface(m_graphics_renderer, m_graphics_surface);
  // if (!m_graphics_texture) CERROR("failed to create texture: %s", SDL_GetError());
  // SDL_FreeSurface(m_graphics_surface);

  // SDL_RenderCopy(m_graphics_renderer, m_graphics_texture, NULL, NULL);
  // SDL_RenderPresent(m_graphics_renderer);
  // SDL_DestroyTexture(m_graphics_texture);

  SDL_GL_SwapWindow(m_graphics_window);

  /* Wait for next frame */
  static double last = 0;
  double step = (1. / m_graphics_maxFps);
  double now = time_getTime();
  // double now = SDL_GetTicks() / 1000.;
  double wait = step - (now - last);
  last += step;
  if (wait > 0) {
    // SDL_Delay(wait * 1000.);
    time_sleep(wait);
  } else {
    /* stops flahing when drawing only once */
    /* SDL_GL_SwapWindow(m_graphics_window); */
    last = now;
  }
}
