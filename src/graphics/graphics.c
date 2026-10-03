//
// Created by Lexi on 24/07/2026.
//


#if defined(__arm__) || defined(__aarch64__)




#else

    #ifdef __CC65__
        #include "cat65/vgc7.c"

    #else

        //#include "./xlib_driver/xlib_driver.h"
    #endif

#endif




void GRAPHICS_PUT_TILE(int x, int y, int tile){
    vgc_put_tile(x,y,tile);
}
