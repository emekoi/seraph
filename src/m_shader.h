// /**
//  * Copyright (c) 2017 emekoi
//  *
//  * This library is free software; you can redistribute it and/or modify it
//  * under the terms of the MIT license. See LICENSE for details.
//  */
//
//
// #ifndef M_SHADER_H
// #define M_SHADER_H
//
// #include <GL/glew.h>
//
// #define shader_use(self) glUseProgram(self->program)
//
// // #define UniformS(size, type) glUniform##size##type
// // #define UniformV(size, type)
//
// typedef struct {
//   GLuint vertex, fragment;
//   GLuint program;
// } Shader;
//
// // enum {
// //   SCALAR,
// //   VECTOR
// // };
//
// Shader *shader_fromString(const char *vertex, const char *v, const char *fragment, const char *f);
// Shader *shader_fromFile(const char *vertex, const char *fragment);
// void shader_setAttribute(Shader *self, const char *name, int size, int type, int norm, int s, void *p);
// // void shader_setUniform(Shader *self, const char *name, void *data);
// void shader_destroy(Shader *self);
//
// #endif
