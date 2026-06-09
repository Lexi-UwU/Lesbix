// Use the address that actually reacted!
#define PL110_BASE 0x10120000

#define LCD_TIMING0 (*(volatile unsigned int *)(PL110_BASE + 0x00))
#define LCD_TIMING1 (*(volatile unsigned int *)(PL110_BASE + 0x04))
#define LCD_TIMING2 (*(volatile unsigned int *)(PL110_BASE + 0x08))
#define LCD_UPBASE (*(volatile unsigned int *)(PL110_BASE + 0x10))
#define LCD_CONTROL (*(volatile unsigned int *)(PL110_BASE + 0x18))

void initialise_lcd() {
    // This will now target the ACTUAL brain at 0x10120000
    LCD_CONTROL = 0;
    LCD_TIMING0 = 0x3F1F3F9C;
    LCD_TIMING1 = 0x090B61DF;
    LCD_TIMING2 = 0x067F1800;

    // IMPORTANT: Now that the brain is at 0x1012,
    // move the CANVAS (framebuffer) to a safe place in RAM
    LCD_UPBASE = 0x08000000;

    LCD_CONTROL = 0x829;


    *(volatile unsigned short *)0x1012001A = 0xFFFF;
}
