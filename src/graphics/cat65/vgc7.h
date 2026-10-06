#ifndef VGC7_DRIVER
#define VGC7_DRIVER

void vgc_set_color(unsigned char color_idx, unsigned char color_val);
void vgc_put_tile(unsigned char x, unsigned char y, unsigned char tile_id);
void vgc_enable_display(unsigned char bank);
void vgc_output();

#endif
