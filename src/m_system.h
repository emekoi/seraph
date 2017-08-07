/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_SYSTEM_H
#define M_SYSTEM_H

#include <SDL/SDL.h>
#include "util.h"

enum {
  RESIZE,
  KEY_UP,
  KEY_DOWN,
  KEY_MOVE,
};

typedef struct {
  int type;
  union {
    struct { size_t width, height; } resize;
    struct { char *key;            } keyup;
    struct { char *key, *chr;      } keydown;
  };
  struct event_t *next;
} event_t;


void system_poll();

/* event handlers */
void __graphics_on_event(event_t *e);
void __keyboard_on_event(event_t *e);

#endif
