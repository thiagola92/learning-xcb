#include <stdio.h>
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
  printf("status->%i\n", setup->status);
  printf("pad0->%i\n", setup->pad0);
  printf("protocol_major_version->%i\n", setup->protocol_major_version);
  printf("protocol_minor_version->%i\n", setup->protocol_minor_version);
  printf("length->%i\n", setup->length);
  printf("release_number->%i\n", setup->release_number);
  printf("resource_id_base->%i\n", setup->resource_id_base);
  printf("resource_id_mask->%i\n", setup->resource_id_mask);
  printf("motion_buffer_size->%i\n", setup->motion_buffer_size);
  printf("vendor_len->%i\n", setup->vendor_len);
  printf("maximum_request_length->%i\n", setup->maximum_request_length);
  printf("roots_len->%i\n", setup->roots_len);
  printf("pixmap_formats_len->%i\n", setup->pixmap_formats_len);
  printf("image_byte_order->%i\n", setup->image_byte_order);
  printf("bitmap_format_bit_order->%i\n", setup->bitmap_format_bit_order);
  printf("bitmap_format_scanline_unit->%i\n",
         setup->bitmap_format_scanline_unit);

  xcb_disconnect(connection);

  return 0;
}