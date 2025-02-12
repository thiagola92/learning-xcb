#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xinput.h>
#include <xcb/xproto.h>

int main() {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);

  if (xcb_connection_has_error(connection)) {
    printf("Error\n");
    xcb_disconnect(connection);

    return 1;
  }

  const char *extension_name = "XInputExtension";
  xcb_query_extension_cookie_t extension_cookie =
      xcb_query_extension(connection, strlen(extension_name), extension_name);
  xcb_query_extension_reply_t *extension_reply =
      xcb_query_extension_reply(connection, extension_cookie, NULL);

  if (!extension_reply || !extension_reply->present) {
    printf("XInputExtension is not present\n");
    free(extension_reply);
    return 0;
  }

  free(extension_reply);

  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;
  xcb_window_t window_id = xcb_generate_id(connection);
  uint32_t window_mask = XCB_CW_BACK_PIXEL;
  uint32_t window_values[] = {screen->white_pixel};

  // Raw keycode doesn't help a lot, so we need to map it to a keysym.
  // First, lets prepare a struct to translate keys in the future.
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(connection);

  xcb_create_window(connection, XCB_COPY_FROM_PARENT, window_id, screen->root,
                    0, 0, 300, 300, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual, window_mask, window_values);
  xcb_map_window(connection, window_id);

  typedef struct mask_t {
    xcb_input_device_id_t deviceid;
    uint16_t mask_len;
    xcb_input_xi_event_mask_t event;
  } mask_t;

  mask_t mask = {XCB_INPUT_DEVICE_ALL_MASTER, 1,
                 XCB_INPUT_XI_EVENT_MASK_RAW_KEY_PRESS |
                     XCB_INPUT_XI_EVENT_MASK_RAW_KEY_RELEASE};
  xcb_void_cookie_t cookie =
      xcb_input_xi_select_events(connection, screen->root, 1, (void *)&mask);
  xcb_generic_error_t *error = xcb_request_check(connection, cookie);

  if (error) {
    printf("Failed to listen keyboard, error_code=%d\n", error->error_code);
    free(error);
    return 0;
  }

  xcb_flush(connection);

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    xcb_ge_event_t *extesion_event = (xcb_ge_event_t *)event;

    switch (extesion_event->event_type) {
    case XCB_INPUT_RAW_KEY_PRESS:
      xcb_input_raw_key_press_event_t *e0 =
          (xcb_input_raw_key_press_event_t *)extesion_event;

      // Translate raw keycode to keysym.
      xcb_keysym_t k0 = xcb_key_symbols_get_keysym(keysyms, e0->detail, 0);

      printf("keysym=%i\n", k0);

      break;
    case XCB_INPUT_RAW_KEY_RELEASE:
      xcb_input_raw_key_release_event_t *e1 =
          (xcb_input_raw_key_release_event_t *)extesion_event;

      // Translate raw keycode to keysym.
      xcb_keysym_t k1 = xcb_key_symbols_get_keysym(keysyms, e0->detail, 0);

      printf("keysym=%i\n", k1);

      break;
    default:
      break;
    }

    free(event);
  }

  xcb_disconnect(connection);

  return 0;
}