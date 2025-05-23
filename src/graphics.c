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
#include "utilities.h"

double camera_scale(double screen_w, double screen_h, world_t *world) {
    return MIN(screen_w, screen_h * MAX_SCREEN_RATIO) / world->level_width;
}

rect_t camera_transform(double screen_w, double screen_h, world_t *world, rect_t rect) {
    double scale = camera_scale(screen_w, screen_h, world);
    return (rect_t){rect.x * scale + screen_w / 2, (rect.y + world->camera_offset) * scale + screen_h / 2, rect.w * scale, rect.h * scale};
}

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, rect_t rect) {
    SDL_FRect sdl_rect = {rect.x - rect.w / 2, rect.y - rect.h / 2, rect.w, rect.h};
    SDL_RenderCopyF(renderer, texture, NULL, &sdl_rect);
}

/**
 * \brief La fonction applique la texture du fond sur le renderer lié à l'écran de jeu
 * \param renderer le renderer
 * \param screen_w la largeur de l'écran
 * \param screen_h la hauteur de l'écran
 * \param world les données du monde
 * \param scroll_offset le décalage de la caméra
 * \param texture la texture liée au fond
 */
void draw_background(SDL_Renderer *renderer, double screen_w, double screen_h, world_t *world, SDL_Texture *texture, double scroll_offset) {
    double scale = camera_scale(screen_w, screen_h, world);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    double rect_height = screen_w * h / w;
    double initial_rect_y = scroll_offset * scale + screen_h / 2 - rect_height / 2;
    double start_rect_y = positive_fmod(initial_rect_y, rect_height) - rect_height;
    int n = MIN(ceil(screen_h / rect_height) + 2, screen_h * 2); // Limiter le nombre d'itérations pour s'assurer de ne pas créer une boucle infinie
    // On dessine plusieurs copies de la texture pour créer un effet de défilement continu et remplir tout l'écran
    for (int i = 0; i < n; i++) {
        SDL_FRect rect = {0.0, start_rect_y + rect_height * i, screen_w, rect_height};
        SDL_RenderCopyF(renderer, texture, NULL, &rect);
    }
}

/**
 * \brief La fonction rafraichit l'écran en fonction de l'état des données du monde
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param window la fenêtre de jeu
 * \param renderer le renderer lié à l'écran de jeu
 * \param world les données du monde
 * \param resources les ressources
 */
void draw_graphics(const char *exe_dir, SDL_Window *window, SDL_Renderer *renderer, resources_t *resources, world_t *world) {
    clear_renderer(renderer);

    int screen_w_int, screen_h_int;
    SDL_GetWindowSize(window, &screen_w_int, &screen_h_int);
    double screen_w = screen_w_int, screen_h = screen_h_int;

    refresh_font(exe_dir, resources, screen_w * FONT_SIZE);

    if (world->game_state == GAME_STATE_SPLASH_SCREEN) {
        draw_background(renderer, screen_w, screen_h, world, resources->splash_screen_texture, 0.0);
    }

    if (world->game_state == GAME_STATE_LEVEL_COMPLETE_SCREEN) {
        draw_background(renderer, screen_w, screen_h, world, resources->background_texture, 0);

        char message[32];
        sprintf(message, "Level %d complete!", world->current_level + 1);
        draw_text(renderer, screen_w / 2, screen_h / 2, true, resources->font, message);
    }

    if (world->game_state == GAME_STATE_END_SCREEN) {
        draw_background(renderer, screen_w, screen_h, world, resources->background_texture, 0);

        if (world->has_won) {
            draw_text(renderer, screen_w / 2, screen_h / 2, true, resources->font, "You won!");
        } else {
            draw_text(renderer, screen_w / 2, screen_h / 2, true, resources->font, "You lost!");
        }
    }

    if (world->game_state == GAME_STATE_PLAYING) {
        draw_background(renderer, screen_w, screen_h, world, resources->background_texture, world->camera_offset * BACKGROUND_SPEED);

        SDL_SetTextureAlphaMod(resources->spaceship_texture, (world->invincible ? 0.5 : 1.0) * 255);
        draw_texture(renderer, resources->spaceship_texture, camera_transform(screen_w, screen_h, world, world->spaceship_rect));

        double flame_intensity = CLAMP(-world->spaceship_speed_y / MAX_USUAL_SPEED, 0.0, 1.0);
        double flame_rect_w = world->spaceship_rect.w * FLAME_SCALE * flame_intensity;
        double flame_rect_h = flame_rect_w * resources->flame_surface->h / resources->flame_surface->w;
        rect_t flame_rect = {world->spaceship_rect.x, world->spaceship_rect.y + world->spaceship_rect.h / 2 + flame_rect_h / 2, flame_rect_w, flame_rect_h};
        SDL_SetTextureAlphaMod(resources->flame_texture, flame_intensity * (world->invincible ? 0.5 : 1.0) * 255);
        draw_texture(renderer, resources->flame_texture, camera_transform(screen_w, screen_h, world, flame_rect));

        draw_texture(renderer, resources->finish_line_texture, camera_transform(screen_w, screen_h, world, world->finish_line_rect));

        for (size_t i = 0; i < world->meteorites_count; i++) {
            draw_texture(renderer, resources->meteorite_texture, camera_transform(screen_w, screen_h, world, world->meteorite_rects[i]));
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
