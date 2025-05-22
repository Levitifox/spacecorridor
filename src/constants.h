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
 * \brief Largeur de l'écran de jeu
 */
#define SCREEN_WIDTH 400

/**
 * \brief Hauteur de l'écran de jeu
 */
#define SCREEN_HEIGHT 640

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
 * \brief Hauteur de la ligne d'arrivée
 */
#define FINISH_LINE_HEIGHT 0.75

/**
 * \brief Vitesse initiale de déplacement vertical des éléments du jeu
 */
#define CRUSING_SPEED 0.0025

/**
 * \brief Vitesse de déplacement avec les touches
 */
#define MOVING_SPEED 0.003

/**
 * \brief Vitesse de défilement de l'arrière-plan
 */
#define BACKGROUND_SPEED 0.5

#endif
