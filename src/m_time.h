/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_TIME_H
#define M_TIME_H

int time_getNow(void);
int time_getTime(void);
int time_sleep(int t);
void time_step(void);
double time_getDelta(void);
double time_getAverage(void);
int time_getFps(void);

#endif
