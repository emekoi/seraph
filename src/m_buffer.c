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


static int sr_BufferMemory(sr_Buffer *b, const void *data, int len) {
  int w, h;
  void *pixels = stbi_load_from_memory(
    data, len, &w, &h, NULL, STBI_rgb_alpha);
  if (!pixels) {
    return -1;
  }
  b = sr_newBuffer(w, h);
  if (!b) {
    zfree(pixels);
    return -1;
  }
  sr_loadPixels(b, pixels, SR_FMT_RGBA);
  zfree(pixels);
  return 0;
}



sr_Buffer *sr_BufferFile(const char *filename) {
  sr_Buffer *b = new_srBuffer();
  size_t len;
  void *data = fs_read(filename, &len);
  if (!data) {
    CERROR("could not open file '%s'", filename);
  }
  int err = sr_BufferMemory(b, data, len);
  free(data);
  if (err) {
    CERROR("could not load buffer");
  }
  return b;
}


sr_Buffer *sr_BufferString(const char *str) {
  size_t len = strlen(str);
  sr_Buffer *b = new_srBuffer();
  int err = sr_BufferMemory(b, str, len);
  if (err) CERROR("could not load sr_Buffer");
  return b;
}


sr_Buffer *sr_BufferBlank(int w, int h) {
  sr_Buffer *b = sr_newBuffer(w, h);
  sr_clear(b, sr_pixel(0, 0, 0, 0));
  if (!b) CERROR("could not create sr_Buffer");
  return b;
}
