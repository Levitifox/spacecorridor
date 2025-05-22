#include "sdl2-light.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief La fonction initialise la SDL.
 * Elle crée la fenêtre du jeu ainsi que le renderer
 *  \param window la fenêtre du jeu
 *  \param renderer le renderer
 * \param width largeur de l'écran de jeu
 * \param height hauteur de l'écran de jeu
 * \return -1 en cas d'erreur, 0 sinon
 */
int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur initialisation de la SDL : %s\n", SDL_GetError());
        return -1;
    }
    // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        fprintf(stderr, "Erreur initialisation de la IMG : %s\n", IMG_GetError());
        return -1;
    }
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI, window, renderer) != 0) {
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
 * \param image_surface_out un pointeur vers un pointeur de surface, si non NULL, la surface de l'image chargée sera stockée ici
 * \param success un pointeur vers un booléen, si non NULL, il sera mis à true si l'image a été chargée avec succès, false sinon
 * \return la image chargée
 */
SDL_Texture *load_image(SDL_Renderer *renderer, const char *exe_path, const char *path, SDL_Surface **image_surface_out, bool *success) {
    char *full_path = exe_path != NULL ? concat_paths(exe_path, path) : strdup(path);
    SDL_Surface *image_surface = IMG_Load(full_path);
    if (image_surface == NULL) {
        if (success) {
            *success = false;
        } else {
            fprintf(stderr, "Erreur pendant chargement image : %s\n", IMG_GetError());
            free(full_path);
        }
        return NULL;
    }
    SDL_Surface *converted_image_surface = SDL_ConvertSurfaceFormat(image_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(image_surface);
    if (converted_image_surface == NULL) {
        if (success) {
            *success = false;
        } else {
            fprintf(stderr, "Impossible d'optimiser l'image! Erreur SDL : %s\n", SDL_GetError());
            free(full_path);
        }
        return NULL;
    }
    SDL_Texture *texture = renderer == NULL ? NULL : SDL_CreateTextureFromSurface(renderer, converted_image_surface);
    if (image_surface_out != NULL) {
        *image_surface_out = converted_image_surface;
    } else {
        SDL_FreeSurface(converted_image_surface);
    }
    if (renderer != NULL && texture == NULL) {
        if (success) {
            *success = false;
        } else {
            fprintf(stderr, "Erreur pendant creation de la texture liee a l'image chargee : %s\n", SDL_GetError());
            free(full_path);
        }
        return NULL;
    }
    free(full_path);
    if (success) {
        *success = true;
    }
    return texture;
}

Uint32 get_pixel_RGBA32(SDL_Surface *surface, size_t x, size_t y) {
    if (!surface || surface->format->BitsPerPixel != 32 || x >= (size_t)surface->w || y >= (size_t)surface->h) {
        fprintf(stderr, "get_pixel_RGBA8888: paramètre invalide ou hors bornes (%p, %zu, %zu)\n", (void *)surface, x, y);
        return 0;
    }
    return ((Uint32 *)surface->pixels)[y * surface->pitch / 4 + x];
}

/**
 * \brief La fonction nettoie une texture en mémoire
 * \param texture la texture à nettoyer
 */
void clean_texture(SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
}

/**
 * \brief La fonction nettoie une surface en mémoire
 * \param surface la surface à nettoyer
 */
void clean_surface(SDL_Surface *surface) {
    SDL_FreeSurface(surface);
}

/**
 * \brief La fonction vide le contenu graphique du renderer lié à l'écran de jeu
 * \param renderer le renderer de l'écran
 */
void clear_renderer(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
}

/**
 * \brief La fonction met à jour l'écran avec le contenu du renderer
 * \param renderer le renderer de l'écran
 */
void update_screen(SDL_Renderer *renderer) {
    SDL_RenderPresent(renderer);
}

/**
 * \brief La fonction nettoie le renderer et la fenêtre du jeu en mémoire
 * \param renderer le renderer à nettoyer
 * \param window la fenêtre à nettoyer
 */
void clean_sdl(SDL_Renderer *renderer, SDL_Window *window) {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
