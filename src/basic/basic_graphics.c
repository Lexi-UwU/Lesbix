#include "basic_graphics.h"




#include "../graphics/graphics.h"


static int BASIC_GRAPHICS_MODE = 0;



//Only graphics mode is 0: Tile mode
void LESBIX_BASIC_GRAPHICS_SET_MODE(int mode){

    BASIC_GRAPHICS_MODE = mode;


}


void LESBIX_BASIC_GRAPHICS_SET_TILE(int x, int y, int tile){

    GRAPHICS_PUT_TILE(x,y,tile);


}

void LESBIX_BASIC_GRAPHICS_SET_PIXEL(int x, int y, int r, int g, int b){

    GRAPHICS_PUT_PIXEL(x,y,r,g,b);
}

