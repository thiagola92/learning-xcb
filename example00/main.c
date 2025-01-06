#include <stdio.h>
#include <xcb/xcb.h>

int main() {
  // XCB use sufix "_t" for types.
  xcb_connection_t *connection = xcb_connect(NULL, NULL);

  if (xcb_connection_has_error(connection)) {
    printf("Error");
  } else {
    printf("Ok");
    xcb_disconnect(connection);
  }

  // Needs to disconnect to free memory even on errors.
  xcb_disconnect(connection);

  return 0;
}