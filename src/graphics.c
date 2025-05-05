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
#include "sdl2-ttf-light.h"

/**
 * \brief La fonction nettoie les ressources
 * \param resources Les ressources
 */
void clean_resources(resources_t *resources) {
    clean_texture(resources->background);
    clean_texture(resources->spaceship);
    clean_texture(resources->ligne);
    clean_texture(resources->meteorite);
    clean_font(resources->font);
}

/**
 * \brief La fonction initialise les ressources nécessaires à l'affichage graphique du jeu
 * \param renderer le renderer correspondant à l'écran de jeu
 * \param resources Les ressources du jeu
 */
void init_resources(SDL_Renderer *renderer, resources_t *resources) {
    init_ttf();
    resources->background = load_image("resources/space-background.png", renderer);
    resources->spaceship = load_image("resources/spaceship.png", renderer);
    resources->ligne = load_image("resources/finish_line.png", renderer);
    resources->meteorite = load_image("resources/meteorite.png", renderer);
    resources->font = load_font("resources/COOPBL.ttf", 28);
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
void apply_background(SDL_Renderer *renderer, SDL_Texture *texture, int scroll_offset) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    /* Deux copies pour un défilement continu */
    SDL_Rect dest1 = {0, scroll_offset, w, h};
    SDL_Rect dest2 = {0, scroll_offset - h, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dest1);
    SDL_RenderCopy(renderer, texture, NULL, &dest2);
}

/**
 * \brief Dessiner les meteos
 * \param renderer le renderer
 * \param world les données du monde
 * \param texture la texture liée au fond
 */
void apply_wall(int h, int w, SDL_Renderer *renderer, SDL_Texture *texture, world_t *world) {
    double px = world->murs->x;
    double py = world->murs->y;
    int meteo = 192;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            apply_texture(texture, renderer, px - (w * METEORITE_SIZE) / 2 + j * METEORITE_SIZE, py - (h * METEORITE_SIZE) / 2 + i * METEORITE_SIZE + meteo);
        }
    }
}

/**
 * \brief Applique les murs de météorites sur le renderer
 * \param renderer Le renderer
 * \param world Les données du monde
 * \param texture La texture des murs
 */
void apply_walls(SDL_Renderer *renderer, world_t *world, SDL_Texture *texture) {
    for (size_t i = 0; i < world->murs_count; i++) {
        // calculer le nombre de météorites à dessiner horizontalement et verticalement
        int num_meteos_x = world->murs[i].w / METEORITE_SIZE;
        int num_meteos_y = world->murs[i].h / METEORITE_SIZE;

        // calculer les positions x et y de départ des météorites
        double start_x = world->murs[i].x - world->murs[i].w / 2;
        double start_y = world->murs[i].y - world->murs[i].h / 2;

        // dessine les météorites
        for (int j = 0; j < num_meteos_y; j++) {
            for (int k = 0; k < num_meteos_x; k++) {
                SDL_Rect rect = {start_x + k * METEORITE_SIZE, start_y + j * METEORITE_SIZE, METEORITE_SIZE, METEORITE_SIZE};
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            }
        }
    }
}

/**
 * \brief La fonction rafraichit l'écran en fonction de l'état des données du monde
 * \param renderer le renderer lié à l'écran de jeu
 * \param world les données du monde
 * \param resources les ressources
 */
void refresh_graphics(SDL_Renderer *renderer, world_t *world, resources_t *resources) {
    clear_renderer(renderer);

    int bg_w, bg_h;
    SDL_QueryTexture(resources->background, NULL, NULL, &bg_w, &bg_h);
    int scroll_offset = (int)(world->time_since_game_start * BACKGROUND_SPEED) % bg_h;
    apply_background(renderer, resources->background, scroll_offset);

    apply_sprite(renderer, resources->spaceship, &world->spaceship);
    apply_sprite(renderer, resources->ligne, &world->ligne);

    // afficher les ressources uniquement si le jeu n'est pas terminé
    if (!world->gameover) {
        apply_walls(renderer, world, resources->meteorite);
    }

    /* Mise à jour du temps écoulé et affichage */
    {
        char timeText[64];
        sprintf(timeText, "Time: %.2f s", world->time_since_game_start / 1000.0);
        // Affichage en haut à gauche
        apply_text(renderer, 10, 10, 150, 30, timeText, resources->font);
    }

    /* Message de fin de partie */
    if (world->gameover) {
        int center_x = SCREEN_WIDTH / 2 - 75;
        int center_y = SCREEN_HEIGHT / 2;
        if (world->has_won) {
            apply_text(renderer, center_x, center_y, 150, 30, "You won!", resources->font);
        } else {
            apply_text(renderer, center_x, center_y, 150, 30, "You lost!", resources->font);
        }
    }

    // Met à jour l'affichage
    update_screen(renderer);
}
