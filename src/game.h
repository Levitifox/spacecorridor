/**
 * \file game.h
 * \brief Fichier contenant les structures et déclarations de fonctions pour la logique du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#ifndef __GAME_H__
#define __GAME_H__

#include "constants.h"
#include "sdl2-light.h"

struct sprite_s {
    int x;
    int y;
    int h;
    int w;
};
typedef struct sprite_s sprite_t;

/**
 * \brief Représentation du monde du jeu
 */
struct world_s {
    sprite_t spaceship;
    int gameover;   /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_t ligne; /*!< La ligne d'arrivée */
    sprite_t mur;
    int speed;
    int down; /*!< La ligne est en bas */
};

/**
 * \brief Type qui correspond aux données du monde
 */
typedef struct world_s world_t;

void init_sprite(sprite_t *sprite, int x, int y, int w, int h);

void print_sprite(char *name, sprite_t *sprite);

void init_data(world_t *world);

void clean_data(world_t *world);

int is_game_over(world_t *world);

void update_data(world_t *world);

void handle_events(SDL_Event *event, world_t *world);

#endif
