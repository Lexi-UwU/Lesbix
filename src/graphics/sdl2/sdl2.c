#include <SDL2/SDL.h>
#include <stdio.h>
#include <stddef.h>

// Global pointers for the window and renderer
SDL_Window* global_window = NULL;
SDL_Renderer* global_renderer = NULL;

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

    // Try hardware acceleration first, then fallback to software
    global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_SOFTWARE);
    if (!global_renderer) {
        global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_SOFTWARE);
    }

    if (!global_renderer) {
        printf("Renderer Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void SDL2_UPDATE(void) {
    if (!global_window || !global_renderer) return;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            // In a real app, you'd set LESBIX_RUNNING = 0 here
            // But for now, we just handle the event to keep the window responsive
        }
    }

    // Solid Blue Background
    SDL_SetRenderDrawColor(global_renderer, 0, 128, 255, 255);
    SDL_RenderClear(global_renderer);
    SDL_RenderPresent(global_renderer);
}
