/**
 * \file main.c
 * \brief Programme principal
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 1er mai 2025
 */

#include "constants.h"
#include "game.h"
#include "graphics.h"
#include "sdl2-light.h"

/**
 * \brief fonction qui nettoie le jeu: nettoyage de la partie graphique (SDL), nettoyage des textures, nettoyage des données
 * \param window la fenêtre du jeu
 * \param renderer le renderer
 * \param textures les textures
 * \param world le monde
 */
void clean(SDL_Window *window, SDL_Renderer *renderer, textures_t *textures, world_t *world) {
    clean_data(world);
    clean_textures(textures);
    clean_sdl(renderer, window);
}

/**
 * \brief fonction qui initialise le jeu: initialisation de la partie graphique (SDL), chargement des textures, initialisation des données
 * \param window la fenêtre du jeu
 * \param renderer le renderer
 * \param textures les textures
 * \param world le monde
 */
void init(SDL_Window **window, SDL_Renderer **renderer, textures_t *textures, world_t *world) {
    init_sdl(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    init_data(world);
    init_textures(*renderer, textures);
}

/**
 * \brief programme principal qui implémente la boucle du jeu
 */
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    world_t world;
    textures_t textures;
    SDL_Renderer *renderer;
    SDL_Window *window;

    // initialisation du jeu
    init(&window, &renderer, &textures, &world);

    while (!is_game_over(&world)) { // tant que le jeu n'est pas fini
        SDL_Event event;

        // gestion des évènements
        handle_events(&event, &world);

        // mise à jour des données liée à la physique du monde
        update_data(&world);

        // rafraichissement de l'écran
        refresh_graphics(renderer, &world, &textures);

        // pause de 10 ms pour controler la vitesse de rafraichissement
        pause(10);
    }

    // nettoyage final
    clean(window, renderer, &textures, &world);

    return 0;
}
