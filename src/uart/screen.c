#include "../graphics/graphics.h"
#include "screen.h"




// --- Functions ---


static int cursor_x;
static int cursor_y;




void screen_init(){
    GRAPHICS_INIT();
}

void screen_send(char c) {
    if (c == 0x0A) {
        cursor_y = cursor_y + 1;
        cursor_x = 0;
        //vgc_put_tile(cursor_x, cursor_y, 'O');
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        GRAPHICS_PUT_TILE(cursor_x, cursor_y, c);
        cursor_x = cursor_x + 1;
    }

    //vgc_output();
}
void screen_print(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        screen_send(s[i]);
    }
}

