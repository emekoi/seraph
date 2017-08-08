/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include "m_buffer.h"
#include "m_font.h"

#define ASSERT(cond, ...) do { \
  if (!(cond)) { \
    fprintf(stderr, "[ASSERT FAILED]:%s %s:%d: %s(): ", #cond, __FILE__, __LINE__, __func__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(EXIT_FAILURE); \
  } \
} while(0);

#define CERROR(...) do { \
  fprintf(stderr, "[CERROR]: %s:%d %s(): ", __FILE__, __LINE__, __func__); \
  fprintf(stderr, __VA_ARGS__); \
  fprintf(stderr, "\n"); \
  exit(EXIT_FAILURE); \
} while(0);

#define UNUSED(x)       ((void) (x))
#define MIN(a, b)       ((b) < (a) ? (b) : (a))
#define MAX(a, b)       ((b) > (a) ? (b) : (a))
#define CLAMP(x, a, b)  (MAX(a, MIN(x, b)))
#define LERP(a, b, p)   ((a) + ((b) - (a)) * (p))
#define RECT(b) sr_rect(0, 0, buffer_getWidth(b), buffer_getHeight(b))

void *zrealloc(void *ptr, size_t size);
void zfree(void *ptr);
int opt_number(int x, int y);
char *opt_string(char *x, char *y);
int round_int(double x);

#endif
