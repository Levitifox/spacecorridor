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
#include "sdl2-light.h"

void print_rect(char *name, rect_t rect) {
    printf("Rect \"%s\" : %gx%g+%g+%g\n", name, rect.w, rect.h, rect.x, rect.y);
}

/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */
void init_data(world_t *world) {
    world->gameover = false;
    world->speed = INITIAL_SPEED;
    world->start_time = SDL_GetTicks64();
    world->last_frame_time = SDL_GetTicks64();
    world->has_won = false;
    world->invincible = false;
    world->current_level = 1;

    int ship_x = SCREEN_WIDTH / 2;
    int ship_y = SCREEN_HEIGHT - SHIP_SIZE;

    world->spaceship = (rect_t){ship_x, ship_y, SHIP_SIZE, SHIP_SIZE};
    print_rect("spaceship", world->spaceship);

    // Initialisation du niveau
    init_level(world);
}

/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */
void clean_data(world_t *world) {
    free(world->murs);
}

/**
 * \brief La fonction indique si le jeu est fini en fonction des données du monde
 * \param world les données du monde
 * \return true si le jeu est fini, false sinon
 */
bool is_game_over(world_t *world) {
    return world->gameover;
}

/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param world les données du monde
 */
void update_data(world_t *world) {
    world->time_since_game_start = SDL_GetTicks64() - world->start_time;
    world->time_since_last_frame = SDL_GetTicks64() - world->last_frame_time;
    world->last_frame_time = SDL_GetTicks64();

    if (world->gameover) {
        return;
    }

    // Collision avec la ligne d'arrivée
    if (rects_collide(world->spaceship, world->ligne)) {
        if (world->current_level < level_count) {
            printf("Level %d complete!\n", world->current_level);
            world->current_level++;
            // Réinitialiser la position pour un nouveau niveau
            int ship_x = SCREEN_WIDTH / 2;
            int ship_y = SCREEN_HEIGHT - SHIP_SIZE;
            world->spaceship = (rect_t){ship_x, ship_y, SHIP_SIZE, SHIP_SIZE};
            print_rect("spaceship", world->spaceship);
            // Libérer la mémoire des murs du niveau précédent
            free(world->murs);
            // Réinitialisation du niveau
            init_level(world);
            return;
        } else {
            world->gameover = true;
            world->has_won = true;
            printf("You finished in %.2f s!\n", world->time_since_game_start / 1000.0);
            return;
        }
    }

    // Mise à jour de la position de la ligne d'arrivée
    world->ligne.y += world->speed * world->time_since_last_frame;

    // Mise à jour des murs et vérifications des limites du vaisseau
    update_walls(world);
    check_left_boundary(world->spaceship);
    check_right_boundary(world->spaceship);

    if (!world->invincible) {
        // Collision entre le vaisseau et le mur de météorites
        for (size_t i = 0; i < world->murs_count; i++) {
            if (rects_collide(world->spaceship, world->murs[i])) {
                world->gameover = true;
                printf("You lost!\n");
                return;
            }
        }
    }

    // Gestion des mouvements
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_Q]) {
        world->spaceship.x -= MOVING_STEP * world->time_since_last_frame;
    }
    if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
        world->spaceship.x += MOVING_STEP * world->time_since_last_frame;
    }
    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_Z]) {
        world->spaceship.y -= MOVING_STEP * world->time_since_last_frame;
    }
    if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
        world->spaceship.y += MOVING_STEP * world->time_since_last_frame;
    }
}

/**
 * \brief Met à jour la position des murs de météorites
 * \param world Les données du monde
 */
void update_walls(world_t *world) {
    for (size_t i = 0; i < world->murs_count; i++) {
        world->murs[i].y += world->speed * world->time_since_last_frame;
    }
}

/**
 * \brief La fonction gère les évènements ayant eu lieu et qui n'ont pas encore été traités
 * \param event paramètre qui contient les événements
 * \param world les données du monde
 */
void handle_events(SDL_Event *event, world_t *world) {
    while (SDL_PollEvent(event)) {
        // Si l'utilisateur a cliqué sur le X de la fenêtre
        if (event->type == SDL_QUIT) {
            // On indique la fin du jeu
            world->gameover = true;
        }
        // Quitter (Echap)
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                world->gameover = true;
            }
            if (event->key.keysym.sym == SDLK_i) {
                world->invincible = !world->invincible;
            }
        }
    }
}

/**
 * \brief Vérifie et corrige la position du vaisseau s'il dépasse la limite gauche de l'écran
 * \param spaceship Le rect du vaisseau
 */
void check_left_boundary(rect_t spaceship) {
    // Si le bord gauche du vaisseau (position x - moitié largeur) est inférieur à 0
    if (spaceship.x - spaceship.w / 2 < 0) {
        spaceship.x = spaceship.w / 2;
    }
}

/**
 * \brief Vérifie et corrige la position du vaisseau s'il dépasse la limite droite de l'écran
 * \param spaceship Le rect du vaisseau
 */
void check_right_boundary(rect_t spaceship) {
    // Si le bord droit du vaisseau (position x + moitié largeur) est supérieur à la largeur de l'écran
    if (spaceship.x + spaceship.w / 2 > SCREEN_WIDTH) {
        spaceship.x = SCREEN_WIDTH - spaceship.w / 2;
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
