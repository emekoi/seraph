/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_CHADER_H
#define M_CHADER_H

#include <SDL2/SDL.h>

#include "util.h"
#include "lib/fs/fs.h"

#include "m_time.h"
#include "m_buffer.h"
#include "m_graphics.h"

void __init(void);
void __draw(void);

void onInit();
void onDraw();

#endif
