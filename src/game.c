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

void init_sprite(sprite_t *sprite, double x, double y, double w, double h) {
    sprite->x = x;
    sprite->y = y;
    sprite->w = w;
    sprite->h = h;
}

void print_sprite(char *name, sprite_t *sprite) {
    printf("Sprite \"%s\" : %gx%g+%g+%g\n", name, sprite->w, sprite->h, sprite->x, sprite->y);
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
    world->invisible = false;
    world->current_level = 1;

    int ship_x = SCREEN_WIDTH / 2;
    int ship_y = SCREEN_HEIGHT - SHIP_SIZE;

    init_sprite(&world->spaceship, ship_x, ship_y, SHIP_SIZE, SHIP_SIZE);
    print_sprite("spaceship", &world->spaceship);

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
    if (sprites_collide(&world->spaceship, &world->ligne)) {
        if (world->current_level < level_count) {
            printf("Level %d complete!\n", world->current_level);
            world->current_level++;
            // Réinitialiser la position pour un nouveau niveau
            int ship_x = SCREEN_WIDTH / 2;
            int ship_y = SCREEN_HEIGHT - SHIP_SIZE;
            init_sprite(&world->spaceship, ship_x, ship_y, SHIP_SIZE, SHIP_SIZE);
            print_sprite("spaceship", &world->spaceship);
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
    check_left_boundary(&world->spaceship);
    check_right_boundary(&world->spaceship);

    // Collision entre le vaisseau et le mur de météorites
    for (size_t i = 0; i < world->murs_count; i++) {
        // Si on appuie sur I il devient invisible
        if (world->invisible == false) {
            if (sprites_collide(&world->spaceship, &world->murs[i])) {
                world->gameover = true;
                printf("You lost!\n");
                return;
            }
        }
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
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
            world->gameover = true;
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
    if (keystate[SDL_SCANCODE_I]) {
        world->invisible = !world->invisible;
    }
}

/**
 * \brief Vérifie et corrige la position du vaisseau s'il dépasse la limite gauche de l'écran
 * \param spaceship Le sprite du vaisseau
 */
void check_left_boundary(sprite_t *spaceship) {
    // Si le bord gauche du vaisseau (position x - moitié largeur) est inférieur à 0
    if (spaceship->x - spaceship->w / 2 < 0) {
        spaceship->x = spaceship->w / 2;
    }
}

/**
 * \brief Vérifie et corrige la position du vaisseau s'il dépasse la limite droite de l'écran
 * \param spaceship Le sprite du vaisseau
 */
void check_right_boundary(sprite_t *spaceship) {
    // Si le bord droit du vaisseau (position x + moitié largeur) est supérieur à la largeur de l'écran
    if (spaceship->x + spaceship->w / 2 > SCREEN_WIDTH) {
        spaceship->x = SCREEN_WIDTH - spaceship->w / 2;
    }
}

/**
 * \brief Indique si deux sprites sont en collision
 * \param sp1 Le premier sprite
 * \param sp2 Le deuxième sprite
 * \return true s'il y a collision, false sinon
 */
bool sprites_collide(sprite_t *sp1, sprite_t *sp2) {
    return MAX(sp1->x - sp1->w / 2, sp2->x - sp2->w / 2) <= MIN(sp1->x + sp1->w / 2, sp2->x + sp2->w / 2) &&
           MAX(sp1->y - sp1->h / 2, sp2->y - sp2->h / 2) <= MIN(sp1->y + sp1->h / 2, sp2->y + sp2->h / 2);
}
