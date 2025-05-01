/**
 * \file game.c
 * \brief Fichier contenant les implémentations des fonctions pour la logique du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#include "game.h"
#include "constants.h"
#include "sdl2-light.h"

void init_sprite(sprite_t *sprite, int x, int y, int w, int h) {
    sprite->x = x;
    sprite->y = y;
    sprite->w = w;
    sprite->h = h;
}

void print_sprite(char *name, sprite_t *sprite) {
    printf("Sprite \"%s\" : %dx%d+%d+%d\n", name, sprite->w, sprite->h, sprite->x, sprite->y);
}

/**
 * \brief La fonction initialise les données du monde du jeu
 * \param world les données du monde
 */
void init_data(world_t *world) {
    // on n'est pas à la fin du jeu
    world->gameover = 0;
    world->speed = INITIAL_SPEED;
    world->down = 1;

    int ship_x = SCREEN_WIDTH / 2;
    int ship_y = SCREEN_HEIGHT - SHIP_SIZE;

    init_sprite(&world->spaceship, ship_x, ship_y, SHIP_SIZE, SHIP_SIZE);
    print_sprite("spaceship", &world->spaceship);

    init_sprite(&world->ligne, ship_x, FINISH_LINE_HEIGHT, SCREEN_WIDTH, FINISH_LINE_HEIGHT);
    print_sprite("ligne", &world->ligne);

    /* Mur de météorites centré, 3 × 7 */
    init_sprite(&world->mur, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3 * METEORITE_SIZE, 7 * METEORITE_SIZE);
    print_sprite("mur", &world->mur);
}

/**
 * \brief La fonction nettoie les données du monde
 * \param world les données du monde
 */
void clean_data(world_t *world) {
    /* utile uniquement si vous avez fait de l'allocation dynamique (malloc); la fonction ici doit permettre de libérer la mémoire (free) */
}

/**
 * \brief La fonction indique si le jeu est fini en fonction des données du monde
 * \param world les données du monde
 * \return 1 si le jeu est fini, 0 sinon
 */
int is_game_over(world_t *world) {
    return world->gameover;
}

/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param les données du monde
 */
void update_data(world_t *world) {
    if (world->down == 1) {
        // la ligne va vers la vers bas
        if (world->ligne.y < SCREEN_HEIGHT - SHIP_SIZE * 2) {
            world->ligne.y = world->ligne.y + world->speed;
        } else {
            world->down = 0;
        }
        if (world->mur.y < SCREEN_HEIGHT - SHIP_SIZE * 2) {
            world->mur.y = world->mur.y + world->speed;
        }

    } else {
        // la ligne va vers la vers haut
        if (world->ligne.y > 0) {
            world->ligne.y = world->ligne.y - world->speed;
        } else {
            world->down = 1;
        }
        if (world->mur.y > 0) {
            world->mur.y = world->mur.y - world->speed;
        }
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
            world->gameover = 1;
        }

        // si une touche est appuyée
        if (event->type == SDL_KEYDOWN) {
            // si la touche appuyée est 'D'
            // if (event->key.keysym.sym == SDLK_d) {
            //     printf("La touche D est appuyée\n");
            // }

            // Gestion des mouvements
            if (event->key.keysym.sym == SDLK_q || event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_LEFT) {
                world->spaceship.x = world->spaceship.x - MOVING_STEP;
            }
            if (event->key.keysym.sym == SDLK_d || event->key.keysym.sym == SDLK_RIGHT) {
                world->spaceship.x = world->spaceship.x + MOVING_STEP;
            }
            if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_DOWN) {
                world->spaceship.y = world->spaceship.y + MOVING_STEP;
            }
            if (event->key.keysym.sym == SDLK_z || event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP) {
                world->spaceship.y = world->spaceship.y - MOVING_STEP;
            }

            // Quitter (Echap)
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                world->gameover = 1;
            }
        }
    }
}
