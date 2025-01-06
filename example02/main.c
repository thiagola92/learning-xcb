#include <stdio.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

int main() {
  int screen_number; // xcb_connect() will fill this value.
  xcb_connection_t *connection = xcb_connect(NULL, &screen_number);

  if (xcb_connection_has_error(connection)) {
    printf("Error");
    xcb_disconnect(connection);

    return 1;
  }

  // Iterate until find our screen.
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

  int i;
  for (i = 0; i < screen_number; i++) {
    xcb_screen_next(&iter);
  }

  xcb_screen_t *screen = iter.data;

  xcb_disconnect(connection);

  return 0;
}