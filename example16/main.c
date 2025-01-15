#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
  uint32_t window_values[] = {screen->white_pixel,
                              XCB_EVENT_MASK_ENTER_WINDOW |
                                  XCB_EVENT_MASK_LEAVE_WINDOW};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 300, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, window_mask, window_values);
  xcb_map_window(connection, window_id);
  xcb_flush(connection);

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_ENTER_NOTIFY:
      xcb_enter_notify_event_t *e0 = (xcb_enter_notify_event_t *)event;
      printf("root_x=%i root_y=%i event_x=%i event_y=%i\n", e0->root_x,
             e0->root_y, e0->event_x, e0->event_y);

      break;
    case XCB_LEAVE_NOTIFY:
      xcb_leave_notify_event_t *e1 = (xcb_leave_notify_event_t *)event;
      printf("root_x=%i root_y=%i event_x=%i event_y=%i\n", e1->root_x,
             e1->root_y, e1->event_x, e1->event_y);

      break;
    default:
      break;
    }

    free(event);
  }

  xcb_disconnect(connection);

  return 0;
}