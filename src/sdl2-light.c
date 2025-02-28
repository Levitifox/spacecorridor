#include "sdl2-light.h"
#include <stdio.h>
#include <stdlib.h>

int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        fprintf(stderr, "Erreur initialisation de la IMG : %s\n", IMG_GetError());
        return -1;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer) != 0) {
        fprintf(stderr, "Erreur lors de la creation de l'image et du renderer : %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *load_image(const char path[], SDL_Renderer *renderer) {
    SDL_Surface *image_surface = IMG_Load(path);
    if (image_surface == NULL) {
        fprintf(stderr, "Erreur pendant chargement image : %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Surface *window_surface = SDL_GetWindowSurface(SDL_RenderGetWindow(renderer));
    if (window_surface == NULL) {
        printf("Impossible d'obtenir la surface! Erreur SDL : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Surface *converted_image_surface = SDL_ConvertSurface(image_surface, window_surface->format, 0);
    SDL_FreeSurface(image_surface);
    if (converted_image_surface == NULL) {
        printf("Impossible d'optimiser l'image! Erreur SDL : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetColorKey(converted_image_surface, SDL_TRUE, SDL_MapRGB(converted_image_surface->format, 255, 0, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, converted_image_surface);
    SDL_FreeSurface(converted_image_surface);
    if (texture == NULL) {
        fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee : %s\n", SDL_GetError());
        return NULL;
    }
    return texture;
}

void apply_texture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
    SDL_Rect dst = {0, 0, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x;
    dst.y = y;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void clean_texture(SDL_Texture *texture) {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
}

void clear_renderer(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
}

void update_screen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

void pause(int time) {
    SDL_Delay(time);
}

void clean_sdl(SDL_Renderer *renderer, SDL_Window *window) {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
