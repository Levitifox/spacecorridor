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
    world->start_time = SDL_GetTicks64();
    world->has_won = false;

    int ship_x = SCREEN_WIDTH / 2;
    int ship_y = SCREEN_HEIGHT - SHIP_SIZE;

    init_sprite(&world->spaceship, ship_x, ship_y, SHIP_SIZE, SHIP_SIZE);
    print_sprite("spaceship", &world->spaceship);

    init_sprite(&world->ligne, SCREEN_WIDTH / 2, -960, SCREEN_WIDTH, FINISH_LINE_HEIGHT);
    print_sprite("ligne", &world->ligne);

    // Initialisation des murs de météorites pour former des couloirs
    init_walls(world);
}

/**
 * \brief Initialise les positions des murs de météorites pour former des couloirs
 * \param world Les données du monde
 */
void init_walls(world_t *world) {
    // Caractéristiques des murs
    // clang-format off
    int wall_positions[6][4] = {
        {48, 0, 96, 192}, // x, y, largeur, hauteur pour le mur
        {252, 0, 96, 192},
        {16, -352, 32, 160},
        {188, -352, 224, 160},
        {48, -672, 96, 192},
        {252, -672, 96, 192}
    };
    // clang-format on

    // Initialisation des murs
    for (int i = 0; i < 6; i++) {
        init_sprite(&world->murs[i], wall_positions[i][0], wall_positions[i][1], wall_positions[i][2], wall_positions[i][3]);
        print_sprite("mur", &world->murs[i]);
    }
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

    if (world->gameover) {
        return;
    }

    // Collision avec la ligne d'arrivée
    if (sprites_collide(&world->spaceship, &world->ligne)) {
        world->gameover = true;
        world->has_won = true;
        printf("You finished in %.2f s!\n", world->time_since_game_start / 1000.0);
        return;
    }

    // Mise à jour de la position de la ligne d'arrivée
    if (world->ligne.y < SCREEN_HEIGHT - FINISH_LINE_HEIGHT) {
        world->ligne.y = world->ligne.y + world->speed;
    } else {
        // Une fois la ligne en bas, on la maintient pour déclencher la collision
        world->ligne.y = SCREEN_HEIGHT - FINISH_LINE_HEIGHT;
    }

    // Mise à jour des murs et vérifications des limites du vaisseau
    update_walls(world);
    check_left_boundary(&world->spaceship);
    check_right_boundary(&world->spaceship);

    // Collision entre le vaisseau et le mur de météorites
    for (int i = 0; i < 6; i++) {
        if (sprites_collide(&world->spaceship, &world->murs[i])) {
            world->gameover = true;
            printf("You lost!\n");
            return;
        }
    }
}

/**
 * \brief Met à jour la position des murs de météorites
 * \param world Les données du monde
 */
void update_walls(world_t *world) {
    for (int i = 0; i < 6; i++) {
        // Mettre à jour uniquement si le mur est visible
        if (world->murs[i].is_visible) {
            world->murs[i].y += world->speed;
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
    return MAX(sp1->x - sp1->w / 2, sp2->x - sp2->w / 2) <= MIN(sp1->x + sp1->w / 2, sp2->x + sp2->w / 2) &&
           MAX(sp1->y - sp1->h / 2, sp2->y - sp2->h / 2) <= MIN(sp1->y + sp1->h / 2, sp2->y + sp2->h / 2);
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
