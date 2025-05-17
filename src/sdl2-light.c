#include "sdl2-light.h"
#include "utilities.h"
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

/**
 * \brief La fonction charge une image
 * \param renderer le renderer lié à l'écran de jeu
 * \param exe_path le chemin de l'exécutable, utilisé pour construire le chemin complet du fichier
 * \param path le chemin du fichier correspondant à l'image
 * \return la image chargée
 */
SDL_Texture *load_image(SDL_Renderer *renderer, const char *exe_path, const char *path) {
    char *full_path = exe_path != NULL ? concat_paths(exe_path, path) : strdup(path);
    SDL_Surface *image_surface = IMG_Load(full_path);
    if (image_surface == NULL) {
        fprintf(stderr, "Erreur pendant chargement image : %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Surface *converted_image_surface = SDL_ConvertSurfaceFormat(image_surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(image_surface);
    if (converted_image_surface == NULL) {
        fprintf(stderr, "Impossible d'optimiser l'image! Erreur SDL : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, converted_image_surface);
    SDL_FreeSurface(converted_image_surface);
    if (texture == NULL) {
        fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee : %s\n", SDL_GetError());
        return NULL;
    }
    free(full_path);
    return texture;
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

void clean_sdl(SDL_Renderer *renderer, SDL_Window *window) {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
