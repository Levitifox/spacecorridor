/**
 * \file graphics.c
 * \brief Fichier contenant les implémentations des fonctions pour l'affichage graphique
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#include "graphics.h"
#include "constants.h"
#include "game.h"
#include "sdl2-light.h"

/**
 * \brief La fonction nettoie les textures
 * \param textures les textures
 */
void clean_textures(textures_t *textures) {
    clean_texture(textures->background);
    clean_texture(textures->spaceship);
    clean_texture(textures->ligne);
    clean_texture(textures->meteorite);
}

/**
 * \brief La fonction initialise les textures nécessaires à l'affichage graphique du jeu
 * \param screen la surface correspondant à l'écran de jeu
 * \param textures les textures du jeu
 */
void init_textures(SDL_Renderer *renderer, textures_t *textures) {
    textures->background = load_image("resources/space-background.png", renderer);
    textures->spaceship = load_image("resources/spaceship.png", renderer);
    textures->ligne = load_image("resources/finish_line.png", renderer);
    textures->meteorite = load_image("resources/meteorite.png", renderer);
}

void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_t *sprite) {
    if (sprite->is_visible) {
        SDL_Rect rect = {sprite->x - sprite->w / 2, sprite->y - sprite->h / 2, sprite->w, sprite->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

/**
 * \brief La fonction applique la texture du fond sur le renderer lié à l'écran de jeu
 * \param renderer le renderer
 * \param texture la texture liée au fond
 */
void apply_background(SDL_Renderer *renderer, SDL_Texture *texture) {
    if (texture != NULL) {
        apply_texture(texture, renderer, 0, 0);
    }
}

/**
 * \brief Dessiner les meteos
 * \param renderer le renderer
 * \param world les données du monde
 * \param texture la texture liée au fond
 */
void apply_wall(int h, int w, SDL_Renderer *renderer, SDL_Texture *texture, world_t *world) {
    int px = world->mur.x;
    int py = world->mur.y;
    int meteo = 192;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            apply_texture(texture, renderer, px - (w * METEORITE_SIZE) / 2 + j * METEORITE_SIZE, py - (h * METEORITE_SIZE) / 2 + i * METEORITE_SIZE + meteo);
        }
    }
}

/**
 * \brief La fonction rafraichit l'écran en fonction de l'état des données du monde
 * \param renderer le renderer lié à l'écran de jeu
 * \param world les données du monde
 * \param textures les textures
 */
void refresh_graphics(SDL_Renderer *renderer, world_t *world, textures_t *textures) {
    // on vide le renderer
    clear_renderer(renderer);
    // application des textures dans le renderer
    apply_background(renderer, textures->background);
    apply_texture(textures->background, renderer, 0, 0);
    apply_sprite(renderer, textures->spaceship, &world->spaceship);
    apply_sprite(renderer, textures->ligne, &world->ligne);
    apply_wall(7, 3, renderer, textures->meteorite, world);
    // on met à jour l'écran
    update_screen(renderer);
}
