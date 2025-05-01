/**
 * \file graphics.h
 * \brief Fichier contenant les structures et déclarations de fonctions pour l'affichage graphique
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "constants.h"
#include "game.h"
#include "sdl2-light.h"

/**
 * \brief Représentation pour stocker les textures nécessaires à l'affichage graphique
 */
struct textures_s {
    SDL_Texture *background; /*!< Texture liée à l'image du fond de l'écran. */
    SDL_Texture *spaceship;
    SDL_Texture *ligne;
    SDL_Texture *meteorite;
};

/**
 * \brief Type qui correspond aux textures du jeu
 */
typedef struct textures_s textures_t;

void clean_textures(textures_t *textures);

void init_textures(SDL_Renderer *renderer, textures_t *textures);

void apply_sprite(SDL_Renderer *renderer, SDL_Texture *texture, sprite_t *sprite);

void apply_background(SDL_Renderer *renderer, SDL_Texture *texture);

void apply_wall(int h, int w, SDL_Renderer *renderer, SDL_Texture *texture, world_t *world);

void refresh_graphics(SDL_Renderer *renderer, world_t *world, textures_t *textures);

#endif
