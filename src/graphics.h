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
#include <SDL2/SDL_ttf.h>

/**
 * \brief Représentation pour stocker les textures nécessaires à l'affichage graphique
 */
typedef struct resources_s {
    SDL_Texture *background; /*!< Texture liée à l'image du fond de l'écran. */
    SDL_Texture *spaceship;
    SDL_Texture *ligne;
    SDL_Texture *meteorite;
    TTF_Font *font;
} resources_t;

/**
 * \brief Type qui correspond aux textures du jeu
 */
typedef struct textures_s textures_t;

void clean_resources(resources_t *resources);

void init_resources(SDL_Renderer *renderer, resources_t *resources);

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, rect_t rect);

void draw_background(SDL_Renderer *renderer, SDL_Texture *texture, int scroll_offset);

void draw_walls(SDL_Renderer *renderer, world_t *world, SDL_Texture *texture);

void draw_graphics(SDL_Renderer *renderer, world_t *world, resources_t *resources);

#endif
