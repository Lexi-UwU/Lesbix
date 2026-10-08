#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

// Global pointers and state
SDL_Window* global_window = NULL;
SDL_Renderer* global_renderer = NULL;
bool global_running = true;

int SDL2_INIT(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Error: %s\n", SDL_GetError());
        return 1;
    }

    global_window = SDL_CreateWindow(
        "Lesbix Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!global_window) {
        printf("Window Error: %s\n", SDL_GetError());
        return 1;
    }

    // Try hardware acceleration with VSync first
    global_renderer = SDL_CreateRenderer(
        global_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Fall back to software rendering if hardware initialization fails
    if (!global_renderer) {
        global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_SOFTWARE);
    }

    if (!global_renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

// Prepare the frame (call at start of frame loop)
void SDL2_CLEAR_SCREEN(int r, int g, int b) {
    if (!global_renderer) return;
    SDL_SetRenderDrawColor(global_renderer, r, g, b, 255);
    SDL_RenderClear(global_renderer);
}

// Plot a single pixel
void SDL2_PUT_PIXEL(int x, int y, int r, int g, int b) {
    if (!global_renderer) return;
    SDL_SetRenderDrawColor(global_renderer, r, g, b, 255);
    SDL_RenderDrawPoint(global_renderer, x, y);
}

// Process OS events and display the frame (call at end of frame loop)
void SDL2_UPDATE(void) {
    if (!global_window || !global_renderer) return;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            global_running = false;
        }
    }

    SDL_RenderPresent(global_renderer);
}

// Cleanup resources before exiting
void SDL2_CLEANUP(void) {
    if (global_renderer) SDL_DestroyRenderer(global_renderer);
    if (global_window) SDL_DestroyWindow(global_window);
    SDL_Quit();
}
