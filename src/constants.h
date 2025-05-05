/**
 * \file constants.h
 * \brief Fichier contenant les constantes du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

/**
 * \brief Nombre maximal de frames par seconde
 */
#define MAX_FPS 240

/**
 * \brief Largeur de l'écran de jeu
 */
#define SCREEN_WIDTH 300

/**
 * \brief Hauteur de l'écran de jeu
 */
#define SCREEN_HEIGHT 480

/**
 * \brief Taille d'un vaisseau
 */
#define SHIP_SIZE 32

/**
 * \brief Taille d'un météorite
 */
#define METEORITE_SIZE 32

/**
 * \brief Hauteur de la ligne d'arrivée
 */
#define FINISH_LINE_HEIGHT 20

/**
 * \brief Vitesse de déplacement horizontal du vaisseau
 */
#define MOVING_STEP 0.125

/**
 * \brief Vitesse initiale de déplacement vertical des éléments du jeu
 */
#define INITIAL_SPEED 0.125

/**
 * \brief Vitesse de défilement de l'arrière-plan
 */
#define BACKGROUND_SPEED 0.1

#endif
