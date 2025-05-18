/**
 * \file level.h
 * \brief Fichier les fonctions pour l'initialisation des niveaux du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 14 mai 2025
 */

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "game.h"

void init_levels(const char *exe_dir, world_t *world);

void init_level(const char *exe_dir, world_t *world);

#endif