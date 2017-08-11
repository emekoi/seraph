/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_BUFFER_H
#define M_BUFFER_H

#include "lib/sera/sera.h"

#define sr_BufferWidth(b) b->w
#define sr_BufferHeight(b) b->h

sr_Buffer *new_srBuffer(void);
sr_Buffer *sr_BufferFile(const char *filename);
sr_Buffer *sr_BufferString(const char *str);
sr_Buffer *sr_BufferBlank(int w, int h);

#endif
