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
    sprite->is_visible = true;
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
    world->gameover = false;
    world->speed = INITIAL_SPEED;
    world->down = true;

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
    (void)world;
}

/**
 * \brief La fonction indique si le jeu est fini en fonction des données du monde
 * \param world les données du monde
 * \return 1 si le jeu est fini, 0 sinon
 */
bool is_game_over(world_t *world) {
    return world->gameover;
}

/**
 * \brief La fonction met à jour les données en tenant compte de la physique du monde
 * \param les données du monde
 */
void update_data(world_t *world) {
    if (world->down) {
        // la ligne va vers la vers bas
        if (world->ligne.y < SCREEN_HEIGHT - SHIP_SIZE * 2) {
            world->ligne.y = world->ligne.y + world->speed;
        } else {
            world->down = false;
        }
        if (world->mur.y < SCREEN_HEIGHT - world->mur.h / 2) {
            world->mur.y = world->mur.y + world->speed;
        }

    } else {
        // la ligne va vers la vers haut
        if (world->ligne.y > FINISH_LINE_HEIGHT / 2) {
            world->ligne.y = world->ligne.y - world->speed;
        } else {
            world->down = true;
        }
        if (world->mur.y > world->mur.h / 2) {
            world->mur.y = world->mur.y - world->speed;
        }
    }

    // Vérifications des limites du vaisseau
    check_left_boundary(&world->spaceship);
    check_right_boundary(&world->spaceship);
    // Collision entre le vaisseau et le mur de météorites
    handle_sprites_collision(&world->spaceship, &world->mur, world, true);
    // Collision entre le vaisseau et la ligne d'arrivée
    handle_sprites_collision(&world->spaceship, &world->ligne, world, false);
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

        // si une touche est appuyée
        if (event->type == SDL_KEYDOWN) {
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
                world->gameover = true;
            }
        }
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
    // Utilisation de la formule du document : |x1 - x2| <= (w1 + w2) / 2 ET |y1 - y2| <= (h1 + h2) / 2
    return (sp1->x - sp2->x) < (sp1->w + sp2->w) / 2 && abs(sp1->y - sp2->y) < (sp1->h + sp2->h) / 2;
}

/**
 * \brief Gère la collision entre deux sprites
 * \param sp1 Le premier sprite
 * \param sp2 Le deuxième sprite
 * \param world Les données du monde
 * \param make_disappear Si vrai, le premier sprite (sp1) disparaît en cas de collision
 */
void handle_sprites_collision(sprite_t *sp1, sprite_t *sp2, world_t *world, bool make_disappear) {
    // Si les deux sprites entrent en collision
    if (sprites_collide(sp1, sp2)) {
        // Alors la vitesse de déplacement verticale dans le monde devient nulle
        world->speed = 0;
        if (make_disappear) {
            sp1->is_visible = false;
        }
    }
}
