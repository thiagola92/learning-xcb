#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

int main() {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);

  if (xcb_connection_has_error(connection)) {
    printf("Error\n");
    xcb_disconnect(connection);

    return 1;
  }

  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;
  xcb_window_t window_id = xcb_generate_id(connection);
  uint32_t window_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t window_values[] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 300, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, window_mask, window_values);
  xcb_map_window(connection, window_id);
  xcb_flush(connection);

  // To listen to all key inputs (even when the windows is not focus),
  // we need to grab the keyboards focus.
  xcb_grab_keyboard_cookie_t keyboard_cookie =
      xcb_grab_keyboard(connection, 0, screen->root, XCB_CURRENT_TIME,
                        XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);

  // We need to get the result from attempting getting the focus.
  xcb_grab_keyboard_reply_t *reply =
      xcb_grab_keyboard_reply(connection, keyboard_cookie, NULL);

  // Check for error.
  if (!reply || reply->status != XCB_GRAB_STATUS_SUCCESS) {
    printf("Failed to grab keyboard, status=%i", reply->status);
    free(reply);
    xcb_disconnect(connection);
    return 0;
  }
  free(reply);

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_KEY_PRESS:
      xcb_key_press_event_t *e = (xcb_key_press_event_t *)event;
      printf("root_x=%i root_y=%i event_x=%i event_y=%i detail=%i\n", e->root_x,
             e->root_y, e->event_x, e->event_y, e->detail);

      break;
    default:
      break;
    }

    free(event);
  }

  xcb_ungrab_keyboard(connection, XCB_CURRENT_TIME);

  xcb_disconnect(connection);

  return 0;
}