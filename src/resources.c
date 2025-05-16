/**
 * \file resources.c
 * \brief Fichier contenant les implémentations des fonctions pour l'initialisation et la gestion des ressources du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 16 mai 2025
 */

#include "resources.h"

/**
 * \brief La fonction initialise les ressources nécessaires à l'affichage graphique du jeu
 * \param renderer le renderer correspondant à l'écran de jeu
 * \param resources Les ressources du jeu
 */
void init_resources(SDL_Renderer *renderer, resources_t *resources) {
    resources->background_texture = load_image("resources/background.png", renderer);
    resources->spaceship_texture = load_image("resources/spaceship.png", renderer);
    resources->finish_line_texture = load_image("resources/finish_line.png", renderer);
    resources->meteorite_texture = load_image("resources/meteorite.png", renderer);
    resources->font = load_font("resources/COOPBL.ttf", 28);
}

/**
 * \brief La fonction nettoie les ressources
 * \param resources Les ressources
 */
void clean_resources(resources_t *resources) {
    clean_texture(resources->background_texture);
    clean_texture(resources->spaceship_texture);
    clean_texture(resources->finish_line_texture);
    clean_texture(resources->meteorite_texture);
    clean_font(resources->font);
}
