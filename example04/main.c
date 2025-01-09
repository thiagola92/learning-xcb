#include <stdint.h>
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

  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;
  xcb_window_t window_id = xcb_generate_id(connection);

  // Set window to listen for events.
  // In this case listen to exposure events.
  uint32_t mask = XCB_CW_EVENT_MASK;
  uint32_t values[] = {XCB_EVENT_MASK_EXPOSURE};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 300, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, mask, values);
  xcb_map_window(connection, window_id);
  xcb_flush(connection);

  // Wait exposure event.
  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      printf("Window got exposed\n");
      break;
    default:
      break;
    }
  }

  xcb_disconnect(connection);

  return 0;
}