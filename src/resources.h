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

void init_resources(SDL_Renderer *renderer, resources_t *resources);

void clean_resources(resources_t *resources);

#endif
