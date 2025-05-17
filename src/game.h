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
#include "resources.h"
#include <stdbool.h>

typedef struct rect_s {
    double x;
    double y;
    double w;
    double h;
} rect_t;

/**
 * \brief Énumération des états du jeu
 */
enum game_state_e {
    GAME_STATE_STARTED,
    GAME_STATE_SPLASH_SCREEN,
    GAME_STATE_PLAYING,
    GAME_STATE_LEVEL_COMPLETE_SCREEN,
    GAME_STATE_END_SCREEN,
    GAME_STATE_QUIT,
};
typedef enum game_state_e game_state_t;

/**
 * \brief Représentation du monde du jeu
 */
typedef struct world_s {
    game_state_t game_state;
    Uint64 last_frame_time;       /*!< Temps de la dernière frame rendue */
    Uint64 time_since_last_frame; /*!< Temps écoulé depuis la dernière frame */
    Uint64 screen_time;
    int splash_screen_sound_channel;
    Uint64 playing_time; /*!< Temps écoulé */
    int current_level;   /*!< Niveau actuel du joueur */
    double camera_offset;
    double speed; /*!< Vitesse du déplacement dans le jeu */
    rect_t spaceship;
    rect_t ligne; /*!< La ligne d'arrivée */
    size_t murs_count;
    rect_t *murs;    /*!< Tableau de murs de météorites */
    bool invincible; /*!< Indique si le joueur est dans un mode invincible */
    bool has_won;    /*!< Indique si le joueur a gagné */
} world_t;

void print_rect(char *name, rect_t rect);

void init_data(world_t *world);

void clean_data(world_t *world);

void update_data(resources_t *resources, world_t *world);

void handle_events(world_t *world);

bool rects_collide(rect_t rect_1, rect_t rect_2);

#endif
