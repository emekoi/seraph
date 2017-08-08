/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <math.h>
#include "m_time.h"

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/time.h>
#endif
#include <SDL2/SDL.h>

static double last = 0;
static double delta = 0;
static double average = 0;
static double avgTimer = 0;
static double avgAcc = 1;
static double avgCount = 1;


int time_getNow(void) {
  double t;
#ifdef _WIN32
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  t = (ft.dwHighDateTime * 4294967296.0 / 1e7) + ft.dwLowDateTime / 1e7;
  t -= 11644473600.0;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  t = tv.tv_sec + tv.tv_usec / 1e6;
#endif
  return t;
}


int time_getTime(void) {
  return SDL_GetTicks() / 1000.;
}


int time_sleep(int t) {
  SDL_Delay(t * 1000.);
  return 0;
}


void time_step(void) {
  int now = time_getTime();
  if (last == 0) last = now;
  delta = now - last;
  last = now;
  avgTimer -= delta;
  avgAcc += delta;
  avgCount += 1;;
  if (avgTimer <= 0) {
    average = avgAcc / avgCount;
    avgTimer = avgTimer + 1;
    avgCount = 0;
    avgAcc = 0;
  }
}


double time_getDelta(void) {
  return delta;
}



double time_getAverage(void){
  return average;
}


int time_getFps(void){
  return floor(1 / average + .5);
}
