//TODO: THIS CANNOT RUN IN TEXT MODE, A SOFTWARE RENDERER IS NEEDED


// Define the start of the VGA text buffer
// Ensure this address matches your specific hardware's memory map
volatile unsigned short * const VGA_BUFFER = (unsigned short *)0x10120000;

// Simple function to write a character at a specific (row, col)
// row: 0-24, col: 0-79
// color_attr: 8-bit attribute byte (e.g., 0x0F for white on black)
void write_vga_char(int row, int col, char c, unsigned char color_attr) {
    int index = (row * 80) + col;
    
    // Combine character and attribute into a 16-bit word
    // Format: [Attribute Byte | Character Byte]
    unsigned short entry = (color_attr << 8) | (unsigned char)c;
    
    VGA_BUFFER[index] = entry;
}

