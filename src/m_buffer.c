#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/sera/sera.h"
#include "util.h"
#include "lib/fs/fs.h"


sr_Buffer *new_srBuffer(void) {
  sr_Buffer *b = zrealloc(NULL, sizeof(*b));
  return b;
}


static sr_Buffer *sr_BufferMemory(const void *data, int len) {
  int w, h;
  void *pixels = stbi_load_from_memory(data, len, &w, &h, NULL, STBI_rgb_alpha);
  if (!pixels) return NULL;
  sr_Buffer *b = sr_newBuffer(w, h);
  if (!b) {
    zfree(pixels);
    return NULL;
  }
  sr_loadPixels(b, pixels, SR_FMT_RGBA);
  zfree(pixels);
  return b;
}



sr_Buffer *sr_newBufferFile(const char *filename) {
  size_t len;
  void *data = fs_read(filename, &len);
  if (!data) CERROR("could not open file '%s'", filename);
  sr_Buffer *b = sr_BufferMemory(data, len);
  free(data);
  if (!b) CERROR("could not load buffer");
  return b;
}


sr_Buffer *sr_newBufferString(const char *str) {
  size_t len = strlen(str);
  sr_Buffer *b = sr_BufferMemory(str, len);
  if (!b) CERROR("could not load sr_Buffer");
  return b;
}


sr_Buffer *sr_newBufferBlank(int w, int h) {
  sr_Buffer *b = sr_newBuffer(w, h);
  if (!b) CERROR("could not create sr_Buffer");
  sr_clear(b, sr_pixel(0, 0, 0, 0));
  return b;
}
