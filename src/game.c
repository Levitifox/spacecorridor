/**
 * \file game.c
 * \brief Fichier contenant les implémentations des fonctions pour la logique du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#include "game.h"
#include "constants.h"
#include "level.h"
#include "utilities.h"

void print_rect(char *name, rect_t rect) {
    printf("Rect \"%s\" : %gx%g%+g%+g\n", name, rect.w, rect.h, rect.x, rect.y);
}

/**
 * \brief La fonction initialise les données du monde du jeu
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param world les données du monde
 */
void init_data(const char *exe_dir, world_t *world) {
    init_levels(exe_dir, world);
    world->game_state = GAME_STATE_STARTED;
    world->last_frame_time = SDL_GetTicks64();
    world->splash_screen_sound_channel = -1;
    world->playing_time = 0;
    world->current_level = 0;
}

void clean_playing_data(world_t *world) {
    free(world->meteorite_rects);
}

/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */
void clean_data(world_t *world) {
    if (world->game_state == GAME_STATE_PLAYING) {
        clean_playing_data(world);
    }
}

void transition_to_splash_screen(resources_t *resources, world_t *world) {
    world->game_state = GAME_STATE_SPLASH_SCREEN;
    world->screen_time = 0;
    world->level_width = 1.0;
    world->level_height = 1.0;
    world->splash_screen_sound_channel = play_sound(resources->splash_screen_sound);
}

void transition_to_playing(const char *exe_dir, resources_t *resources, world_t *world) {
    world->game_state = GAME_STATE_PLAYING;
    world->camera_offset = INITIAL_CAMERA_OFFSET;
    world->has_won = false;
    world->invincible = false;

    world->spaceship_rect = (rect_t){0.0, 0.0, SPACESHIP_SIZE, SPACESHIP_SIZE};
    print_rect("spaceship", world->spaceship_rect);
    world->spaceship_speed_x = 0.0;
    world->spaceship_speed_y = 0.0;

    init_level(exe_dir, resources, world);

    stop_sound(world->splash_screen_sound_channel);
}

void transition_to_level_complete_screen(world_t *world) {
    world->game_state = GAME_STATE_LEVEL_COMPLETE_SCREEN;
    world->screen_time = 0;
    clean_playing_data(world);
    printf("Level %d complete!\n", world->current_level + 1);
}

void transition_to_end_screen_loss(resources_t *resources, world_t *world) {
    world->game_state = GAME_STATE_END_SCREEN;
    world->screen_time = 0;
    world->has_won = false;
    clean_playing_data(world);
    printf("You lost!\n");
    play_sound(resources->loss_sound);
}

void transition_to_end_screen_win(resources_t *resources, world_t *world) {
    world->game_state = GAME_STATE_END_SCREEN;
    world->screen_time = 0;
    world->has_won = true;
    free(world->meteorite_rects);
    printf("You finished in %.2f s!\n", world->playing_time / 1000.0);
    play_sound(resources->win_sound);
}

void transition_to_quit(world_t *world) {
    world->game_state = GAME_STATE_QUIT;
}

/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param world les données du monde
 */
void update_data(const char *exe_dir, resources_t *resources, world_t *world) {
    world->time_since_last_frame = SDL_GetTicks64() - world->last_frame_time;
    world->last_frame_time = SDL_GetTicks64();

    if (world->game_state == GAME_STATE_STARTED) {
        transition_to_splash_screen(resources, world);
    } else if (world->game_state == GAME_STATE_SPLASH_SCREEN) {
        world->screen_time += world->time_since_last_frame;
        if (world->screen_time >= 3000) {
            transition_to_playing(exe_dir, resources, world);
        }
    } else if (world->game_state == GAME_STATE_PLAYING) {
        world->playing_time += world->time_since_last_frame;

        // Mise à jour de l'accélération, la vitesse et la position du vaisseau

        double space_acceleration_x = 0.0;
        double space_acceleration_y = 0.0;

        space_acceleration_y -= CRUSING_SPEED;

        // Gestion des mouvements avec les touches du clavier
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_Q]) {
            space_acceleration_x -= MOVING_SPEED;
        }
        if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
            space_acceleration_x += MOVING_SPEED;
        }
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_Z]) {
            space_acceleration_y -= MOVING_SPEED;
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
            space_acceleration_y += MOVING_SPEED;
        }

        space_acceleration_x -= world->spaceship_speed_x * DRAG_COEFFICIENT;
        space_acceleration_y -= world->spaceship_speed_y * DRAG_COEFFICIENT;

        world->spaceship_speed_x += space_acceleration_x * world->time_since_last_frame;
        world->spaceship_speed_y += space_acceleration_y * world->time_since_last_frame;

        world->spaceship_rect.x += world->spaceship_speed_x * world->time_since_last_frame;
        world->spaceship_rect.y += world->spaceship_speed_y * world->time_since_last_frame;

        world->spaceship_rect.x =
            CLAMP(world->spaceship_rect.x, -world->level_width / 2 + world->spaceship_rect.w / 2, world->level_width / 2 - world->spaceship_rect.w / 2);

        // Mise à jour de la position de la caméra (décroissance exponentielle vets la position du vaisseau)
        world->camera_offset +=
            (-world->spaceship_rect.y + INITIAL_CAMERA_OFFSET - world->camera_offset) * (1.0 - exp(-CAMERA_APPROACH_RATE * world->time_since_last_frame));

        do {
            // Collision avec la ligne d'arrivée
            if (objects_collide(resources->spaceship_surface, world->spaceship_rect, resources->finish_line_surface, world->finish_line_rect)) {
                if (world->current_level == world->level_count - 1) {
                    transition_to_end_screen_win(resources, world);
                    break;
                } else {
                    transition_to_level_complete_screen(world);
                    break;
                }
            }

            if (!world->invincible) {
                // Collision entre le vaisseau et les météorites
                for (size_t i = 0; i < world->meteorites_count; i++) {
                    if (objects_collide(resources->spaceship_surface, world->spaceship_rect, resources->meteorite_surface, world->meteorite_rects[i])) {
                        transition_to_end_screen_loss(resources, world);
                        break;
                    }
                }
            }
        } while (false);
    } else if (world->game_state == GAME_STATE_LEVEL_COMPLETE_SCREEN) {
        world->screen_time += world->time_since_last_frame;
        if (world->screen_time >= 3000) {
            world->current_level++;
            transition_to_playing(exe_dir, resources, world);
        }
    } else if (world->game_state == GAME_STATE_END_SCREEN) {
        world->screen_time += world->time_since_last_frame;
        if (world->screen_time >= 3000) {
            transition_to_quit(world);
        }
    }
}

/**
 * \brief La fonction gère les évènements ayant eu lieu et qui n'ont pas encore été traités
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param resources les ressources
 * \param world les données du monde
 */
void handle_events(const char *exe_dir, resources_t *resources, world_t *world) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Si l'utilisateur a cliqué sur le X de la fenêtre
        if (event.type == SDL_QUIT) {
            // On indique la fin du jeu
            transition_to_quit(world);
        }
        // Quitter (Echap)
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                transition_to_quit(world);
            }
            if (event.key.keysym.sym == SDLK_i && world->game_state == GAME_STATE_PLAYING) {
                world->invincible = !world->invincible;
            }
            if (event.key.keysym.sym == SDLK_SPACE && world->game_state == GAME_STATE_SPLASH_SCREEN) {
                transition_to_playing(exe_dir, resources, world);
            }
        }
    }
}

/**
 * \brief Indique si deux rects sont en collision
 * \param rect_1 Le premier rect
 * \param rect_2 Le deuxième rect
 * \return true s'il y a collision, false sinon
 */
bool rects_collide(rect_t rect_1, rect_t rect_2) {
    return MAX(rect_1.x - rect_1.w / 2, rect_2.x - rect_2.w / 2) <= MIN(rect_1.x + rect_1.w / 2, rect_2.x + rect_2.w / 2) &&
           MAX(rect_1.y - rect_1.h / 2, rect_2.y - rect_2.h / 2) <= MIN(rect_1.y + rect_1.h / 2, rect_2.y + rect_2.h / 2);
}

/**
 * \brief Indique si deux objets sont en collision
 * \param surface_1 La première surface
 * \param rect_1 Le premier rect
 * \param surface_2 La deuxième surface
 * \param rect_2 Le deuxième rect
 * \return true s'il y a collision, false sinon
 */
bool objects_collide(SDL_Surface *surface_1, rect_t rect_1, SDL_Surface *surface_2, rect_t rect_2) {
    if (!rects_collide(rect_1, rect_2)) {
        return false;
    }
    // On itere chaque pixel de la surface 1, on calcule la position de ce pixel dans la systeme de coordonnée global, puis on le transforme dans la surface 2.
    // On dit qu'il y a collision si la somme des alpha des deux pixels est supérieur à 0xFF.
    for (int sx_1 = 0; sx_1 < surface_1->w; sx_1++) {
        for (int sy_1 = 0; sy_1 < surface_1->h; sy_1++) {
            double x = rect_1.x + rect_1.w * ((double)sx_1 / surface_1->w - 0.5);
            double y = rect_1.y + rect_1.h * ((double)sy_1 / surface_1->h - 0.5);
            int sx_2 = round(((x - rect_2.x) / rect_2.w + 0.5) * surface_2->w);
            int sy_2 = round(((y - rect_2.y) / rect_2.h + 0.5) * surface_2->h);
            if (sx_2 >= 0 && sx_2 < surface_2->w && sy_2 >= 0 && sy_2 < surface_2->h) {
                Uint32 pixel_1 = get_pixel_RGBA32(surface_1, sx_1, sy_1);
                Uint32 pixel_2 = get_pixel_RGBA32(surface_2, sx_2, sy_2);
                Uint8 alpha_1 = (pixel_1 >> 24) & 0xFF;
                Uint8 alpha_2 = (pixel_2 >> 24) & 0xFF;
                if (alpha_1 + alpha_2 > 0xFF) {
                    return true;
                }
            }
        }
    }
    // Et vice versa.
    for (int sx_2 = 0; sx_2 < surface_2->w; sx_2++) {
        for (int sy_2 = 0; sy_2 < surface_2->h; sy_2++) {
            double x = rect_2.x + rect_2.w * ((double)sx_2 / surface_2->w - 0.5);
            double y = rect_2.y + rect_2.h * ((double)sy_2 / surface_2->h - 0.5);
            int sx_1 = round(((x - rect_1.x) / rect_1.w + 0.5) * surface_1->w);
            int sy_1 = round(((y - rect_1.y) / rect_1.h + 0.5) * surface_1->h);
            if (sx_1 >= 0 && sx_1 < surface_1->w && sy_1 >= 0 && sy_1 < surface_1->h) {
                Uint32 pixel_2 = get_pixel_RGBA32(surface_2, sx_2, sy_2);
                Uint32 pixel_1 = get_pixel_RGBA32(surface_1, sx_1, sy_1);
                Uint8 alpha_2 = (pixel_2 >> 24) & 0xFF;
                Uint8 alpha_1 = (pixel_1 >> 24) & 0xFF;
                if (alpha_2 + alpha_1 > 0xFF) {
                    return true;
                }
            }
        }
    }
    return false;
}
