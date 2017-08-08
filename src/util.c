/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include <string.h>
#include <math.h>
#include "util.h"


void *zrealloc(void *ptr, size_t size) {
  if (ptr && size == 0) {
    free(ptr);
    return NULL;
  }
  void *p = realloc(ptr, size);
  if (!p) CERROR("out of memory");
  memset(p, 0, size);
  return p;
}


void zfree(void *ptr) {
  zrealloc(ptr, 0);
}


int opt_number(int x, int y) {
  if (x) return x; else return y;
}

char *opt_string(char *x, char *y) {
  if (x) return x; else return y;
}

int round_int(double x) {
  return x >= 0 ? floor(x + .5) : ceil(x - .5);
}
