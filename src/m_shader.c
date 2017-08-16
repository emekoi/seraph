/**
 * Copyright (c) 2017 emekoi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */


#include "m_shader.h"
#include "util.h"
#include "lib/fs/fs.h"


static Shader *shader_new() {
  Shader *self = zrealloc(NULL, sizeof(*self));
  return self;
}

static char *shader_type(GLint type) {
  switch (type) {
    case GL_VERTEX_SHADER:   return "vertex";
    case GL_FRAGMENT_SHADER: return "fragment";
    default:                 return "???";
  }
}


static GLuint compileShader(const GLchar *src, GLint type) {
  /* Compile vertex shader */
  const GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader); GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  char buffer[1024];
  glGetShaderInfoLog(shader, 1024, NULL, buffer);
  if (status != GL_TRUE) CERROR("%s: %s\n", shader_type(type), buffer);
  return shader;
}


Shader *shader_fromString(const char *vertex, const char *fragment) {
  Shader *S = shader_new(); GLuint program;
  S->vertex = compileShader(vertex, GL_VERTEX_SHADER);
  S->fragment = compileShader(fragment, GL_FRAGMENT_SHADER);
  program =  glCreateProgram();
  glAttachShader(program, S->vertex);
  glAttachShader(program, S->fragment);
  glLinkProgram(program);
  S->program = program;
  return S;
}


Shader *shader_fromFile(const char *vertex, const char *fragment) {
  GLchar *v_src = fs_read(vertex, NULL);
  GLchar *f_src = fs_read(fragment, NULL);
  Shader *S = shader_fromString(v_src, f_src);
  zfree(v_src); zfree(f_src);
  return S;
}


void shader_setAttribute(Shader *S, const char *name, int size, int type, int norm) {
  GLint attrib = glGetAttribLocation(S->program, name);
  glVertexAttribPointer(attrib, size, type, norm, 0, 0);
  glEnableVertexAttribArray(attrib);
}


static void set_uniform(GLuint location, int size, int vec, int type) {
  if (vec) {

  } else {

  }
}


void shader_setUniform(Shader *S, const char *name, void *data) {
  GLint uniform = glGetUniformLocation(S->program, name);
  set_uniform(uniform, 0, 0, 0);
  // glUniform3f(uniform, sin(time) * 2, cos(time) * 2, tan(time) * 2);
}


void shader_destroy(Shader *S) {
  glDeleteShader(S->vertex);
  glDeleteShader(S->fragment);
  glDetachShader(S->program, S->vertex);
  glDetachShader(S->program, S->fragment);
  glDeleteProgram(S->program);
  zfree(S);
}
