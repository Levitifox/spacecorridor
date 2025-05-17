/**
 * \file level.c
 * \brief Fichier contenant l'implémentation de l'initialisation de la ligne d'arrivée et des murs selon le niveau courant
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 14 mai 2025
 */

#include "level.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

const int level_count = 3;

/* positions des murs pour chaque niveau */
/* clang-format off */
static const int wall_positions_lvl0[][4] = {
    {48, 0, 96, 192},
    {252, 0, 96, 192},
    {16, -352, 32, 160},
    {188, -352, 224, 160},
    {48, -672, 96, 192},
    {252, -672, 96, 192}
};
static const int wall_positions_lvl1[][4] = {
    {80, 0, 160, 224},
    {268, 0, 64, 224},
    {32, -352, 64, 192},
    {220, -352, 160, 192},
    {112, -704, 224, 192}
};
static const int wall_positions_lvl2[][4] = {
    {64, 32, 128, 128},
    {48, -48, 96, 32},
    {252, -48, 96, 32},
    {236, 32, 128, 128},
    {150, -256, 96, 160},
    {48, -480, 96, 32},
    {252, -480, 96, 32},
    {64, -560, 128, 128},
    {236, -560, 128, 128}
};
/* clang-format on */

/**
 * \brief Initialise les éléments du niveau selon le niveau courant
 * \param world Les données du monde
 */
void init_level(world_t *world) {
    const int (*wall_positions)[4] = NULL;
    int finish_line_y = 0;
    size_t murs_count;

    if (world->current_level == 0) {
        // Niveau 1
        finish_line_y = -960;
        wall_positions = wall_positions_lvl0;
        murs_count = sizeof wall_positions_lvl0 / sizeof wall_positions_lvl0[0];
    } else if (world->current_level == 1) {
        // Niveau 2
        finish_line_y = -960;
        wall_positions = wall_positions_lvl1;
        murs_count = sizeof wall_positions_lvl1 / sizeof wall_positions_lvl1[0];
    } else if (world->current_level == 2) {
        // Niveau 3
        finish_line_y = -850;
        wall_positions = wall_positions_lvl2;
        murs_count = sizeof wall_positions_lvl2 / sizeof wall_positions_lvl2[0];
    } else {
        fprintf(stderr, "Level not found: %d\n", world->current_level);
        exit(1);
    }

    world->ligne = (rect_t){SCREEN_WIDTH / 2, finish_line_y, SCREEN_WIDTH, FINISH_LINE_HEIGHT};
    print_rect("ligne", world->ligne);

    world->murs_count = murs_count;
    world->murs = malloc(sizeof(rect_t) * murs_count);
    for (size_t i = 0; i < murs_count; i++) {
        world->murs[i] = (rect_t){wall_positions[i][0], wall_positions[i][1], wall_positions[i][2], wall_positions[i][3]};
        print_rect("mur", world->murs[i]);
    }
}
