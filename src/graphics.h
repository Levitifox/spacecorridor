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
#include "resources.h"
#include "sdl2-light.h"
#include <SDL2/SDL_ttf.h>

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, rect_t rect);

void draw_background(SDL_Renderer *renderer, double screen_w, double screen_h, world_t *world, SDL_Texture *texture, double scroll_offset);

void draw_graphics(const char *exe_dir, SDL_Window *window, SDL_Renderer *renderer, resources_t *resources, world_t *world);

void wait_for_next_frame(world_t *world);

#endif
