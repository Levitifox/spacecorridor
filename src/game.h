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

struct rect_s {
    double x;
    double y;
    double w;
    double h;
};
typedef struct rect_s rect_t;

/**
 * \brief Représentation du monde du jeu
 */
struct world_s {
    double camera_offset;
    rect_t spaceship;
    bool gameover; /*!< Champ indiquant si l'on est à la fin du jeu */
    rect_t ligne;  /*!< La ligne d'arrivée */
    size_t murs_count;
    rect_t *murs;                 /*!< Tableau de murs de météorites */
    double speed;                 /*!< Vitesse du déplacement dans le jeu */
    Uint64 start_time;            /*!< Temps de début du jeu */
    Uint64 time_since_game_start; /*!< Temps écoulé depuis le début du jeu */
    Uint64 last_frame_time;       /*!< Temps de la dernière frame rendue */
    Uint64 time_since_last_frame; /*!< Temps écoulé depuis la dernière frame */
    bool has_won;                 /*!< Indique si le joueur a gagné */
    bool invincible;              /*!< Indique si le joueur est dans un mode invincible */
    int current_level;            /*!< Niveau actuel du joueur */
};

/**
 * \brief Type qui correspond aux données du monde
 */
typedef struct world_s world_t;

void print_rect(char *name, rect_t rect);

void init_data(world_t *world);

void clean_data(world_t *world);

void update_data(world_t *world);

void handle_events(world_t *world);

bool rects_collide(rect_t rect_1, rect_t rect_2);

#endif
