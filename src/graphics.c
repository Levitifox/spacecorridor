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

rect_t camera_transform(world_t *world, rect_t rect) {
    return (rect_t){rect.x, rect.y + world->camera_offset, rect.w, rect.h};
}

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, rect_t rect) {
    SDL_FRect sdl_rect = {rect.x - rect.w / 2, rect.y - rect.h / 2, rect.w, rect.h};
    SDL_RenderCopyF(renderer, texture, NULL, &sdl_rect);
}

/**
 * \brief La fonction applique la texture du fond sur le renderer lié à l'écran de jeu
 * \param renderer le renderer
 * \param texture la texture liée au fond
 */
void draw_background(SDL_Renderer *renderer, SDL_Texture *texture, int scroll_offset) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    /* Deux copies pour un défilement continu */
    SDL_Rect dest1 = {0, scroll_offset, w, h};
    SDL_Rect dest2 = {0, scroll_offset - h, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dest1);
    SDL_RenderCopy(renderer, texture, NULL, &dest2);
}

/**
 * \brief Applique les murs de météorites sur le renderer
 * \param renderer Le renderer
 * \param world Les données du monde
 * \param texture La texture des murs
 */
void draw_walls(SDL_Renderer *renderer, world_t *world, SDL_Texture *texture) {
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
                /* clang-format off */
                rect_t rect = {
                    start_x + k * METEORITE_SIZE + METEORITE_SIZE / 2,
                    start_y + j * METEORITE_SIZE + METEORITE_SIZE / 2,
                    METEORITE_SIZE,
                    METEORITE_SIZE,
                };
                /* clang-format on */
                draw_texture(renderer, texture, camera_transform(world, rect));
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
void draw_graphics(SDL_Renderer *renderer, world_t *world, resources_t *resources) {
    clear_renderer(renderer);

    int bg_w, bg_h;
    SDL_QueryTexture(resources->background_texture, NULL, NULL, &bg_w, &bg_h);
    int scroll_offset = (int)(world->camera_offset * BACKGROUND_SPEED) % bg_h;
    draw_background(renderer, resources->background_texture, scroll_offset);

    SDL_SetTextureAlphaMod(resources->spaceship_texture, world->invincible ? 128 : 255);
    draw_texture(renderer, resources->spaceship_texture, camera_transform(world, world->spaceship));
    draw_texture(renderer, resources->finish_line_texture, camera_transform(world, world->ligne));

    // Affichage d'un message entre les niveaux
    {
        static int last_level = 1;
        if (!world->gameover && world->current_level > last_level) {
            int center_x = SCREEN_WIDTH / 2 - 125;
            int center_y = SCREEN_HEIGHT / 2;

            char message[32];
            sprintf(message, "Level %d complete!", last_level);
            draw_text(renderer, center_x, center_y, 150, 30, message, resources->font);
            update_screen(renderer);

            pause(3000);
            world->last_frame_time = SDL_GetTicks64();

            last_level = world->current_level;
            clear_renderer(renderer);
        }
    }

    // afficher les ressources uniquement si le jeu n'est pas terminé
    if (!world->gameover) {
        draw_walls(renderer, world, resources->meteorite_texture);
    }

    /* Mise à jour du temps écoulé et affichage */
    {
        char timeText[64];
        sprintf(timeText, "Time: %.2f s", world->playing_time / 1000.0);
        // Affichage en haut à gauche
        draw_text(renderer, 10, 10, 150, 30, timeText, resources->font);
    }

    /* Message de fin de partie */
    if (world->gameover) {
        int center_x = SCREEN_WIDTH / 2 - 75;
        int center_y = SCREEN_HEIGHT / 2;
        if (world->has_won) {
            draw_text(renderer, center_x, center_y, 150, 30, "You won!", resources->font);
        } else {
            draw_text(renderer, center_x, center_y, 150, 30, "You lost!", resources->font);
        }
    }

    // Met à jour l'affichage
    update_screen(renderer);
}
