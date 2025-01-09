#include <stdint.h>
#include <stdio.h>
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
  uint32_t window_values[] = {screen->white_pixel, XCB_EVENT_MASK_EXPOSURE};

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 300, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, window_mask, window_values);
  xcb_map_window(connection, window_id);
  xcb_flush(connection);

  // Create graphical context and set to draw to foreground.
  xcb_gcontext_t gcontext_id = xcb_generate_id(connection);
  uint32_t gcontext_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  uint32_t gcontext_values[] = {screen->black_pixel, 0};
  xcb_create_gc(connection, gcontext_id, screen->root, gcontext_mask,
                gcontext_values);

  // Prepare points do be draw.
  int points_len = 16;
  xcb_point_t points[] = {{10, 30}, {11, 30}, {12, 30}, {13, 30},
                          {14, 30}, {15, 30}, {16, 30}, {17, 30},
                          {18, 30}, {19, 30}, {20, 30}, {21, 30},
                          {22, 30}, {23, 30}, {24, 30}, {25, 30}};

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    switch (event->response_type & ~0x80) {
    case XCB_EXPOSE:
      // Draw point(s).
      // NOTE: while drawable is a "xcb_drawable_t", we pass a "xcb_window_t".
      xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, window_id, gcontext_id,
                     points_len, points);

      xcb_flush(connection);
      break;
    default:
      break;
    }
  }

  xcb_disconnect(connection);

  return 0;
}