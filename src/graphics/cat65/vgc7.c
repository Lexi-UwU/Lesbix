/*
 * Cat65 Video Library
 * Target: Expansion Port 4 (0x4000)
 */

int currentBuffer;

#define VGC_BASE      0x4000
#define VGC_REG_CTRL  ((volatile unsigned char*)0x4003)
#define VGC_VRAM_BASE ((volatile unsigned char*)0x4000)

// --- Video Functions ---

/**
 * Sets the color for a specific palette index.
 * color_idx: 0 to 15
 * color_val: The color ID (e.g., 0x07 for gray, 0x1F for white)
 */
void vgc_set_color(unsigned char color_idx, unsigned char color_val) {
    // Palette is at 0x780 relative to the start of the VRAM
    VGC_VRAM_BASE[0x800 + 0x780 + color_idx] = color_val;
}
/**
 * Places a tile at a specific X, Y coordinate.
 * x: 0 to 63 (approx)
 * y: 0 to 31 (approx)
 * tile_id: The index of the tile from the ROM
 */
void vgc_put_tile(unsigned char x, unsigned char y, unsigned char tile_id) {
    // Target the specific VRAM location.
    // The emulator logic: vram00[relAddress - 0x800]
    // So if we send 0x4800 + index, the emulator sees: (0x4800 + index) - 0x4000 (Base) = 0x800 + index
    // Then 0x800 + index - 0x800 = index. This is perfect.
    unsigned int address = 0x4800 + (x | (y << 6));
    *((volatile unsigned char*)address) = tile_id;
}


void vgc_enable_display(unsigned char bank) {
    // 0x40 = Visible (Force Blanking Off)
    // 0x08 = Bank 1 selection
    unsigned char ctrl_val = 0x40;
    //unsigned char ctrl_val = 0xC0;
    if (bank == 1) {
        ctrl_val |= 0x08;
    }
    *VGC_REG_CTRL = ctrl_val;
}




static void vgc_output(){

    // 1. Calculate the inactive buffer (the one NOT currently displayed)
    int backBuffer = 1 - currentBuffer;

    // 2. Clear ONLY the back buffer
    // You may need to modify fillScreen to accept a buffer index,
    // or just ensure currentBuffer points to the back buffer here.
    int temp = currentBuffer;
    currentBuffer = backBuffer;

    vgc_set_color(0, 0x07 );  // Color 0 -> Gray
    vgc_set_color(15, 0x1F); // Color 15 -> White
    //vgc_enable_display(currentBuffer);
    vgc_enable_display(currentBuffer);



}
