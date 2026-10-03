#include "basic_graphics.h"



#ifdef __CC65__
#include "../graphics/graphics.c"

static int BASIC_GRAPHICS_MODE = 0;



//Only graphics mode is 0: Tile mode
void LESBIX_BASIC_GRAPHICS_SET_MODE(int mode){

    BASIC_GRAPHICS_MODE = mode;


}


void LESBIX_BASIC_GRAPHICS_SET_TILE(int x, int y, int tile){

    BASIC_GRAPHICS_MODE = mode;


}

