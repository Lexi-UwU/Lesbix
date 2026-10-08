//
// Created by Lexi on 24/07/2026.
//

#include "graphics.h"

#if defined(__CC65__) || defined(__CAT65__)
    #include "cat65/vgc7.h"
#elif defined(__DESKTOP__)
    #include "sdl2/sdl2.h"
#elif defined(__ARM_BAREMETAL__) || defined(__arm__)
    //#include "xlib_driver/xlib_driver.h"
#endif


void GRAPHICS_PUT_TILE(int x, int y, int tile){
    #ifdef __CC65__
        vgc_put_tile(x,y,tile);
    #endif

}

void GRAPHICS_UPDATE(){
    #ifdef __CC65__
        // vgc_update();
    #elif defined(__DESKTOP__)
        SDL2_UPDATE();
    #endif
}

void GRAPHICS_INIT(){
    #ifdef __CC65__
        vgc_output();
    #elif defined(__DESKTOP__)
        SDL2_INIT();
    #endif
}




void GRAPHICS_PUT_PIXEL(int x, int y, int r, int g, int b){
    #ifdef __CC65__
        //vgc_put_tile(x,y,tile);
    #elif defined(__DESKTOP__)
        SDL2_PUT_PIXEL(x, y, r, g, b);
    #endif
}
