#include "m_keyboard.h"
#include "lib/map/map.h"

static map_int_t keysDown;
static map_int_t keysPressed;


void __keyboard_open(void) {
  map_init(&keysDown);
  map_init(&keysPressed);
}


void __keyboard_close(void) {
  map_deinit(&keysDown);
  map_deinit(&keysPressed);
}


void __keyboard_on_event(event_t *e) {
  switch (e->type) {
    case KEY_DOWN:
      map_set(&keysDown, e->keydown.key, 1);
      map_set(&keysPressed, e->keydown.key, 1);
      onKeyDown(e->keydown.key, e->keydown.chr);
      break;
    case KEY_UP:
      map_set(&keysDown, e->keyup.key, 0);
      onKeyUp(e->keydown.key);
    break;
  }
}


void __keyboard_reset(void) {
  const char *key;
  map_iter_t iter = map_iter(&keysPressed);
  while ((key = map_next(&keysPressed, &iter))) {
    map_set(&keysPressed, key, 0);
  }
}


int keyboard_isDown(char *_key) {
  char *key = calloc(strlen(_key), sizeof(char));
  sprintf(key, "%s", _key);
  char *k = strtok(key, " ");
  int t = 0;
  while (k != NULL) {
    int *res = map_get(&keysDown, k);
    t += res ? (*res > 0 ? 1 : 0) : 0;
    k = strtok(NULL, " ");
  }
  return t > 0 ? 1 : 0;
}


int keyboard_wasPressed(char *_key) {
  char *key = calloc(strlen(_key), sizeof(char));
  sprintf(key, "%s", _key);
  char *k = strtok(key, " ");
  int t = 0;
  while (k != NULL) {
    int *res = map_get(&keysPressed, k);
    t += res ? (*res > 0 ? 1 : 0) : 0;
    k = strtok(NULL, " ");
  }
  return t > 0 ? 1 : 0;
}