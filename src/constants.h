/**
 * \file constants.h
 * \brief Fichier contenant les constantes du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/**
 * \brief Nombre maximal de frames par seconde
 */
#define MAX_FPS 240

/**
 * \brief Largeur initial de l'écran de jeu
 */
#define INITIAL_SCREEN_WIDTH 400

/**
 * \brief Hauteur initial de l'écran de jeu
 */
#define INITIAL_SCREEN_HEIGHT 640

/**
 * \brief Ratio maximal de l'écran de jeu
 */
#define MAX_SCREEN_RATIO 0.8

/**
 * \brief Taille de la police
 */
#define FONT_SIZE 0.08

/**
 * \brief Décalage initial de la caméra
 */
#define INITIAL_CAMERA_OFFSET 5.0

/**
 * \brief Taille d'un vaisseau
 */
#define SHIP_SIZE 1.0

/**
 * \brief Taille d'un météorite
 */
#define METEORITE_SIZE 1.0

/**
 * \brief Coefficient de la traînée
 */
#define DRAG_COEFFICIENT 0.003

/**
 * \brief Vitesse initiale de déplacement vertical des éléments du jeu
 */
#define CRUSING_SPEED 0.000005

/**
 * \brief Vitesse de déplacement avec les touches
 */
#define MOVING_SPEED 0.00001

/**
 * \brief Vitesse d'approche de la caméra vers le vaisseau
 */
#define CAMERA_APPROACH_RATE 0.001

/**
 * \brief Vitesse de défilement de l'arrière-plan
 */
#define BACKGROUND_SPEED 0.5

#endif
