#define FRAMEBUFFER_BASE 0x08000000 // Match the LCD_UPBASE above
#define SCREEN_WIDTH 640

// Helper to write a 16-bit color to a pixel
void draw_pixel(int x, int y, unsigned short color) {
    // Calculate the memory offset
    unsigned short *ptr = (unsigned short *)FRAMEBUFFER_BASE;
    ptr[(y * SCREEN_WIDTH) + x] = color;
}


void clear_screen(unsigned short color) {
    unsigned short *fb = (unsigned short *)FRAMEBUFFER_BASE;
    // We use a VERY small number here.
    // If this doesn't crash, we can slowly increase it.
    for (int i = 0; i < 13; i++) {
        fb[i] = color;
    }
}


void draw_diagonal() {
    for (int i = 0; i < 400; i++) {
        draw_pixel(i, i, 0xFFFF);
    }
}

