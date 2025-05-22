/**
 * \file utilities.c
 * \brief Fichier contenant les déclarations de fonctions utilitaires
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 17 mai 2025
 */

#include "utilities.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief La fonction calcule le reste positif de la division de a par b
 * \param a le dividende
 * \param b le diviseur
 * \return le reste positif de la division de a par b (enter 0 et b)
 */
double positive_fmod(double a, double b) {
    double r = fmod(a, b);
    return r >= 0.0 ? r : r + fabs(b);
}

char *concat_paths(const char *path_1, const char *path_2) {
    char *full_path = malloc(strlen(path_1) + strlen(path_2) + 2);

    if (full_path == NULL) {
        return NULL;
    }

    strcpy(full_path, path_1);
    strcat(full_path, path_2);
    sprintf(full_path, "%s/%s", path_1, path_2);

    return full_path;
}
