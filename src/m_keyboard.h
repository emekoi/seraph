/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_KEYBOARD_H
#define M_KEYBOARD_H

#include <SDL/SDL.h>
#include "m_system.h"

void __keyboard_open(void);
void __keyboard_close(void);
void __keyboard_reset(void);

int keyboard_isDown(char *_key);
int keyboard_wasPressed(char *_key);

extern void onKeyDown(char *key, char *chr);
extern void onKeyUp(char *key);
#endif