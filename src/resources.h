/**
 * \file resources.h
 * \brief Fichier contenant les structures et déclarations de fonctions pour l'initialisation et la gestion des ressources du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 16 mai 2025
 */

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "sdl2-light.h"
#include "sdl2-ttf-light.h"
#include <SDL_mixer.h>

/**
 * \brief Représentation pour stocker les textures nécessaires à l'affichage graphique
 */
typedef struct resources_s {
    SDL_Surface *splash_screen_surface;
    SDL_Texture *splash_screen_texture;
    SDL_Surface *background_surface;
    SDL_Texture *background_texture;
    SDL_Surface *spaceship_surface;
    SDL_Texture *spaceship_texture;
    SDL_Surface *finish_line_surface;
    SDL_Texture *finish_line_texture;
    SDL_Surface *meteorite_surface;
    SDL_Texture *meteorite_texture;
    Mix_Chunk *splash_screen_sound;
    Mix_Chunk *loss_sound;
    Mix_Chunk *win_sound;
    TTF_Font *font;
} resources_t;

void init_mix();

void clean_mix();

Mix_Chunk *load_sound(const char *exe_path, const char *path);

int play_sound(Mix_Chunk *sound);

void stop_sound(int sound_channel);

void clean_sound(Mix_Chunk *sound);

void init_resources(const char *exe_dir, SDL_Renderer *renderer, resources_t *resources);

void refresh_font(const char *exe_dir, resources_t *resources, int font_size);

void clean_resources(resources_t *resources);

#endif
