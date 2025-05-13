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
#include <stdbool.h>

struct sprite_s {
    double x;
    double y;
    double w;
    double h;
    bool is_visible;
};
typedef struct sprite_s sprite_t;

/**
 * \brief Représentation du monde du jeu
 */
struct world_s {
    sprite_t spaceship;
    bool gameover;  /*!< Champ indiquant si l'on est à la fin du jeu */
    sprite_t ligne; /*!< La ligne d'arrivée */
    size_t murs_count;
    sprite_t *murs;               /*!< Tableau de murs de météorites */
    double speed;                 /*!< Vitesse du déplacement dans le jeu */
    Uint64 start_time;            /*!< Temps de début du jeu */
    Uint64 time_since_game_start; /*!< Temps écoulé depuis le début du jeu */
    Uint64 last_frame_time;       /*!< Temps de la dernière frame rendue */
    Uint64 time_since_last_frame; /*!< Temps écoulé depuis la dernière frame */
    bool has_won;                 /*!< Indique si le joueur a gagné */
    bool invisible;               /*!< Indique si le joueur est dans un mode invisible */
    int current_level;            /*!< Niveau actuel du joueur */
};

/**
 * \brief Type qui correspond aux données du monde
 */
typedef struct world_s world_t;

void init_sprite(sprite_t *sprite, double x, double y, double w, double h);

void print_sprite(char *name, sprite_t *sprite);

void init_data(world_t *world);

void clean_data(world_t *world);

bool is_game_over(world_t *world);

void update_data(world_t *world);

void handle_events(SDL_Event *event, world_t *world);

void check_left_boundary(sprite_t *spaceship);

void check_right_boundary(sprite_t *spaceship);

bool sprites_collide(sprite_t *sp1, sprite_t *sp2);

void handle_sprites_collision(sprite_t *sp1, sprite_t *sp2, world_t *world, bool make_disappear);

void init_walls(world_t *world);

void update_walls(world_t *world);

#endif
