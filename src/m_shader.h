/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#ifndef M_FONT_H
#define M_FONT_H

#include <GL/glew.h>

#define UniformS(size, type) glUniform##size##type
#define UniformV(size, type)

typedef struct {
  GLuint vertex, fragment;
  GLuint program;
} Shader;

enum {
  SCALAR,
  VECTOR
};

Shader *shader_fromString(const char *vertex, const char *fragment);
Shader *shader_fromFile(const char *vertex, const char *fragment);
void shader_setAttribute(Shader *S, const char *name, int size, int type, int norm);
void shader_setUniform(Shader *S, const char *name, void *data);
void shader_destroy(Shader *S);

#endif
