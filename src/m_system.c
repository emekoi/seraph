/**
 * Copyright (c) 2017 emkoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include "m_system.h"

static char keyChar(char c) {
  return (c == '\r') ? '\n' : c;
}

void system_poll() {
  /* Handle events */
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    event_t *event = zrealloc(NULL, sizeof(*event));
    switch (e.type) {
      case SDL_QUIT:
        exit(EXIT_SUCCESS);
        break;

      case SDL_WINDOWEVENT_SIZE_CHANGED:
      case SDL_WINDOWEVENT_RESIZED:
       event->type = RESIZE;
       event->resize.width = e.window.data1;
       event->resize.height = e.window.data2;
       __graphics_on_event(event);
       break;

      // case SDL_KEYDOWN: {
      //   event->type = KEY_DOWN;
      //   char *key = SDL_GetKeyName(e.key.keysym.sym);
      //   event->keydown.key = zrealloc(event->keydown.key, sizeof(char) * strlen(key));
      //   sprintf(event->keydown.key, "%s", key);
      //   int c = keyChar(e.key.keysym.unicode);
      //   if (c) {
      //     event->keydown.chr = zrealloc(event->keydown.chr, sizeof(char));
      //     sprintf(event->keydown.chr, "%c", c);
      //   }
      //   __keyboard_on_event(event);
      //   break;
      // }

      // case SDL_KEYUP:
      //   event->type = KEY_UP;
      //   char *key = SDL_GetKeyName(e.key.keysym.sym);
      //   event->keyup.key = zrealloc(event->keyup.key, sizeof(char) * strlen(key));
      //   sprintf(event->keyup.key, "%s", key);
      //   __keyboard_on_event(event);
      //   break;

    }
  }
}
