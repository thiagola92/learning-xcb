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
  printf("root->%i\n", screen->root);
  printf("white_pixel->%i\n", screen->white_pixel);
  printf("black_pixel->%i\n", screen->black_pixel);
  printf("current_input_masks->%i\n", screen->current_input_masks);
  printf("width_in_pixels->%i\n", screen->width_in_pixels);
  printf("height_in_pixels->%i\n", screen->height_in_pixels);
  printf("width_in_millimeters->%i\n", screen->width_in_millimeters);
  printf("height_in_millimeters->%i\n", screen->height_in_millimeters);
  printf("min_installed_maps->%i\n", screen->min_installed_maps);
  printf("max_installed_maps->%i\n", screen->max_installed_maps);

  xcb_disconnect(connection);

  return 0;
}