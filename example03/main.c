#include <stdio.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

int main() {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);

  if (xcb_connection_has_error(connection)) {
    printf("Error");
    xcb_disconnect(connection);

    return 1;
  }

  // Get the first screen.
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;

  // Create window.
  xcb_window_t window_id = xcb_generate_id(connection);
  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 150, 150, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, 0, NULL);

  // Map to the window, which means that will draw in that window.
  xcb_map_window(connection, window_id);

  // Make sure to send commands before waiting for Ctrl+C.
  xcb_flush(connection);

  // Wait until Ctrl+C is pressed.
  pause();

  xcb_disconnect(connection);

  return 0;
}