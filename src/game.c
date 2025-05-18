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
    world->splash_screen_sound_channel = play_sound(resources->splash_screen_sound);
}

void transition_to_playing(const char *exe_dir, world_t *world) {
    world->game_state = GAME_STATE_PLAYING;
    world->camera_offset = 0.0;
    world->speed = INITIAL_SPEED;
    world->has_won = false;
    world->invincible = false;

    int ship_x = SCREEN_WIDTH / 2;
    int ship_y = SCREEN_HEIGHT - SHIP_SIZE;

    world->spaceship_rect = (rect_t){ship_x, ship_y, SHIP_SIZE, SHIP_SIZE};
    print_rect("spaceship", world->spaceship_rect);

    // Initialisation du niveau
    init_level(exe_dir, world);
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
            transition_to_playing(exe_dir, world);
        }
    } else if (world->game_state == GAME_STATE_PLAYING) {
        world->playing_time += world->time_since_last_frame;

        // Mise à jour de la position de la camera
        world->camera_offset += world->speed * world->time_since_last_frame;

        world->spaceship_rect.y -= MOVING_STEP * world->time_since_last_frame;

        // Gestion des mouvements
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_Q]) {
            world->spaceship_rect.x -= MOVING_STEP * world->time_since_last_frame;
        }
        if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
            world->spaceship_rect.x += MOVING_STEP * world->time_since_last_frame;
        }
        if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_Z]) {
            world->spaceship_rect.y -= MOVING_STEP * world->time_since_last_frame;
        }
        if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
            world->spaceship_rect.y += MOVING_STEP * world->time_since_last_frame;
        }

        world->spaceship_rect.x = CLAMP(world->spaceship_rect.x, world->spaceship_rect.w / 2, SCREEN_WIDTH - world->spaceship_rect.w / 2);

        do {
            // Collision avec la ligne d'arrivée
            if (rects_collide(world->spaceship_rect, world->finish_line_rect)) {
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
                    if (rects_collide(world->spaceship_rect, world->meteorite_rects[i])) {
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
            transition_to_playing(exe_dir, world);
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
 * \param world les données du monde
 */
void handle_events(const char *exe_dir, world_t *world) {
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
                transition_to_playing(exe_dir, world);
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
