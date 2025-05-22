/**
 * \file level.c
 * \brief Fichier les fonctions pour l'initialisation des niveaux du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 14 mai 2025
 */

#include "level.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Initialise l'information sur les niveaux du jeu
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param world les données du monde
 */
void init_levels(const char *exe_dir, world_t *world) {
    bool success = true;
    for (world->level_count = 0; success; world->level_count++) {
        char level_path[30];
        sprintf(level_path, "resources/level_%d.png", world->level_count);
        load_image(NULL, exe_dir, level_path, NULL, &success);
    }
    world->level_count--;
}

/**
 * \brief Initialise les éléments du niveau selon le niveau courant
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param resources les ressources
 * \param world les données du monde
 */
void init_level(const char *exe_dir, resources_t *resources, world_t *world) {
    char level_path[30];
    sprintf(level_path, "resources/level_%d.png", world->current_level);
    SDL_Surface *level_surface;
    load_image(NULL, exe_dir, level_path, &level_surface, NULL);

    world->level_width = level_surface->w;
    world->level_height = level_surface->h;

    world->finish_line_rect = (rect_t){0, -METEORITE_SIZE * level_surface->h, world->level_width,
                                       world->level_width * resources->finish_line_surface->h / resources->finish_line_surface->w};
    print_rect("ligne", world->finish_line_rect);

    world->meteorites_count = 0;
    world->meteorite_rects = malloc(sizeof(rect_t) * level_surface->w * level_surface->h);
    for (size_t x = 0; x < (size_t)level_surface->w; x++) {
        for (size_t y = 0; y < (size_t)level_surface->h; y++) {
            if ((get_pixel_RGBA32(level_surface, x, y) & 0xFFFFFF00) == 0xFFFFFF00) {
                world->meteorite_rects[world->meteorites_count] =
                    (rect_t){(double)x - world->level_width / 2 + 0.5, -(double)(level_surface->h - y - 1), METEORITE_SIZE, METEORITE_SIZE};
                print_rect("meteorite", world->meteorite_rects[world->meteorites_count]);
                world->meteorites_count++;
            }
        }
    }
}
