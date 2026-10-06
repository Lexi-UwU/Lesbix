/* CC65 doesn't have stdbool.h, so define bool manually */
typedef unsigned char bool;
#define true 1
#define false 0

#define ACIA_DATA   (*(volatile unsigned char*) 0x5000)
#define ACIA_STATUS (*(volatile unsigned char*) 0x5001)
#define ACIA_CMD    (*(volatile unsigned char*) 0x5002)
#define ACIA_CTRL   (*(volatile unsigned char*) 0x5003)

char acia_read(void) {
    // Bit 3 (%00001000) indicates the receiver data register is full
    while ((ACIA_STATUS & 0x08) == 0) {
        //return -1;
        // Do nothing, wait for incoming hardware data
    }
    return ACIA_DATA;
}
void acia_wait(void) {
    // wait for ~520 cpu cycles
    __asm__(
        "\tphx \n"
        "\tldx #102 \n"
        "\t@wait:\n"
        "\tdex \n"
        "\tbne @wait\n"
        "\tplx \n"
    );
}

bool acia_has_data(void) {
    return (ACIA_STATUS & 0x08) != 0; // Checking Bit 3
}



void acia_send(char c) {
    ACIA_DATA = c;
    acia_wait();
}
void acia_print(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        acia_send(s[i]);
    }
}

void init_acia(void) {
    //cursor_x = 0;
    //currentBuffer = 0;

    // ACIA Initialization
    ACIA_STATUS = 0;       // ACIA soft reset
    ACIA_CMD  = 0x0b;      // %00001011 (no parity, no echo, TIC 2, Receiver IRQ disabled, DTRB low)
    ACIA_CTRL = 0x1f;      // %00011111 (1 stop bit, word length 8, use baud rate, baud rate 19200)

    acia_send(0x0c);
}

void acia_test(void) {
    acia_print("Hello\0");
    acia_print("\r\0");
    acia_print("World!\0");
}

void acia_clear(void) {
    acia_send(0x0c);
}
