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
#include <libgen.h>

/**
 * \brief fonction qui initialise le jeu: initialisation de la partie graphique (SDL), chargement des ressources, initialisation des données
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param window la fenêtre du jeu
 * \param renderer le renderer
 * \param resources les ressources
 */
void init(const char *exe_dir, SDL_Window **window, SDL_Renderer **renderer, resources_t *resources, world_t *world) {
    init_sdl(window, renderer, INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT);
    init_ttf();
    init_mix();
    SDL_SetWindowTitle(*window, "Spacecorridor"); // Définir le titre de la fenêtre
    init_resources(exe_dir, *renderer, resources);
    init_data(exe_dir, world);
}

/**
 * \brief fonction qui nettoie le jeu: nettoyage de la partie graphique (SDL), nettoyage des textures, nettoyage des données
 * \param window la fenêtre du jeu
 * \param renderer le renderer
 * \param resources les ressources
 * \param world le monde
 */
void clean(SDL_Window *window, SDL_Renderer *renderer, resources_t *resources, world_t *world) {
    clean_data(world);
    clean_resources(resources);
    clean_mix();
    clean_ttf();
    clean_sdl(renderer, window);
}

/**
 * \brief programme principal qui implémente la boucle du jeu
 */
int main(int argc, char **argv) {
    if (argc == 0) {
        fprintf(stderr, "Chemin du programme requis\n");
        exit(1);
    }

    world_t world;
    resources_t resources;
    SDL_Renderer *renderer;
    SDL_Window *window;

    // initialisation du jeu
    char *exe_path = strdup(argv[0]);
    char *exe_dir = strdup(dirname(exe_path));
    free(exe_path);
    init(exe_dir, &window, &renderer, &resources, &world);

    while (true) {
        // gestion des évènements
        handle_events(exe_dir, &resources, &world);

        // mise à jour des données liée à la physique du monde
        update_data(exe_dir, &resources, &world);
        if (world.game_state == GAME_STATE_QUIT) { // tant que le jeu n'est pas fini
            break;
        }

        // rafraichissement de l'écran
        draw_graphics(exe_dir, window, renderer, &resources, &world);

        wait_for_next_frame(&world);
    }

    // nettoyage final
    clean(window, renderer, &resources, &world);
    free(exe_dir);

    return 0;
}
