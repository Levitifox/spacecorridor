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
 * \brief La fonction rafraichit l'écran en fonction de l'état des données du monde
 * \param renderer le renderer lié à l'écran de jeu
 * \param world les données du monde
 * \param resources les ressources
 */
void draw_graphics(SDL_Renderer *renderer, resources_t *resources, world_t *world) {
    clear_renderer(renderer);

    if (world->game_state == GAME_STATE_SPLASH_SCREEN) {
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // plein écran
        SDL_RenderCopy(renderer, resources->splash_screen_texture, NULL, &dest);
    }

    if (world->game_state == GAME_STATE_LEVEL_COMPLETE_SCREEN) {
        draw_background(renderer, resources->background_texture, 0);

        char message[32];
        sprintf(message, "Level %d complete!", world->current_level + 1);
        draw_text(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, true, resources->font, message);
    }

    if (world->game_state == GAME_STATE_END_SCREEN) {
        draw_background(renderer, resources->background_texture, 0);

        if (world->has_won) {
            draw_text(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, true, resources->font, "You won!");
        } else {
            draw_text(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, true, resources->font, "You lost!");
        }
    }

    if (world->game_state == GAME_STATE_PLAYING) {
        int bg_w, bg_h;
        SDL_QueryTexture(resources->background_texture, NULL, NULL, &bg_w, &bg_h);
        int scroll_offset = (int)(world->camera_offset * BACKGROUND_SPEED) % bg_h;
        draw_background(renderer, resources->background_texture, scroll_offset);

        SDL_SetTextureAlphaMod(resources->spaceship_texture, world->invincible ? 128 : 255);
        draw_texture(renderer, resources->spaceship_texture, camera_transform(world, world->spaceship_rect));

        draw_texture(renderer, resources->finish_line_texture, camera_transform(world, world->finish_line_rect));

        for (size_t i = 0; i < world->meteorites_count; i++) {
            draw_texture(renderer, resources->meteorite_texture, camera_transform(world, world->meteorite_rects[i]));
        }

        /* Mise à jour du temps écoulé et affichage */
        {
            char timeText[64];
            sprintf(timeText, "Time: %.2f s", world->playing_time / 1000.0);
            // Affichage en haut à gauche
            draw_text(renderer, 10, 10, false, resources->font, timeText);
        }
    }

    // Met à jour l'affichage
    update_screen(renderer);
}

void wait_for_next_frame(world_t *world) {
    Uint64 now_time = SDL_GetTicks64();
    Uint64 next_frame_time = world->last_frame_time + 1000 / MAX_FPS;
    if (next_frame_time > now_time) {
        SDL_Delay(next_frame_time - now_time);
    }
}
