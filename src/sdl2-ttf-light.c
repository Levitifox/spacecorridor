#include "sdl2-ttf-light.h"

/**
 * \brief La fonction initialise l'environnement TTF
 */
void init_ttf() {
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

/**
 * \brief La fonction charge une police
 * \param path le chemin du fichier correpondant à la police
 * \param font_size la taille de la police
 * \return la police chargée
 */
TTF_Font *load_font(const char *path, int font_size) {
    TTF_Font *font = TTF_OpenFont(path, font_size);
    if (font == NULL) {
        fprintf(stderr, "Erreur pendant chargement font: %s\n", TTF_GetError());
    }
    return font;
}

/**
 * \brief La fonction applique un texte dans une certaine police sur le renderer à une certaine position et avec une certaine dimension
 * \param renderer le renderer
 * \param x abscisse du coin en haut à gauche du texte
 * \param y son abscisse
 * \param w la largeur du message
 * \param h sa hauteur
 * \param text le texte à afficher
 * \param font la police
 */
void apply_text(SDL_Renderer *renderer, int x, int y, int w, int h, const char *text, TTF_Font *font) {
    SDL_Color color = {230, 212, 175, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect dstrect2 = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect2);
}

void clean_font(TTF_Font *font) {
    TTF_CloseFont(font);
}