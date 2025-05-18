#include "sdl2-ttf-light.h"
#include "utilities.h"
#include <SDL.h>
#include <SDL_ttf.h>

/**
 * \brief La fonction initialise l'environnement TTF
 */
void init_ttf() {
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

/**
 * \brief La fonction charge une police
 * \param exe_path le chemin de l'exécutable, utilisé pour construire le chemin complet du fichier
 * \param path le chemin du fichier correspondant à la police
 * \param font_size la taille de la police
 * \return la police chargée
 */
TTF_Font *load_font(const char *exe_path, const char *path, int font_size) {
    char *full_path = exe_path != NULL ? concat_paths(exe_path, path) : strdup(path);
    TTF_Font *font = TTF_OpenFont(full_path, font_size);
    if (font == NULL) {
        fprintf(stderr, "Erreur pendant chargement de la police : %s\n", TTF_GetError());
        free(full_path);
        return NULL;
    }
    free(full_path);
    return font;
}

/**
 * \brief La fonction applique un texte dans une certaine police sur le renderer à une certaine position et avec une certaine dimension
 * \param renderer le renderer
 * \param x abscisse du centre
 * \param y son abscisse
 * \param center centrer
 * \param font la police
 * \param text le texte à afficher
 */
void draw_text(SDL_Renderer *renderer, double x, double y, bool center, TTF_Font *font, const char *text) {
    SDL_Color color = {230, 212, 175, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    int w;
    int h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_FRect sdl_rect;
    if (center) {
        sdl_rect = (SDL_FRect){x - (double)w / 2, y - (double)h / 2, w, h};
    } else {
        sdl_rect = (SDL_FRect){x, y, w, h};
    }
    SDL_RenderCopyF(renderer, texture, NULL, &sdl_rect);
    SDL_DestroyTexture(texture);
}

/**
 * \brief La fonction nettoie une police
 * \param font la police à nettoyer
 */
void clean_font(TTF_Font *font) {
    TTF_CloseFont(font);
}

/**
 * \brief La fonction nettoie l'environnement TTF
 */
void clean_ttf() {
    TTF_Quit();
}
