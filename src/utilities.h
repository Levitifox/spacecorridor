/**
 * \file utilities.h
 * \brief Fichier contenant les déclarations de fonctions utilitaires
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 17 mai 2025
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define CLAMP(x, a, b) (MAX((a), MIN((b), (x))))

double positive_fmod(double a, double b);

char *concat_paths(const char *path_1, const char *path_2);

#endif
