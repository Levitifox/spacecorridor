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
#include "sdl2-ttf-light.h"
#include <SDL_mixer.h>

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
    clean_ttf();
    clean_sdl(renderer, window);
}

/**
 * \brief fonction qui initialise le jeu: initialisation de la partie graphique (SDL), chargement des ressources, initialisation des données
 * \param window la fenêtre du jeu
 * \param renderer le renderer
 * \param resources les ressources
 */
void init(SDL_Window **window, SDL_Renderer **renderer, resources_t *resources) {
    init_sdl(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    init_ttf();
    SDL_SetWindowTitle(*window, "Spacecorridor"); // Définir le titre de la fenêtre
    init_resources(*renderer, resources);
}

/**
 * \brief Affiche l'écran de démarrage en plein écran pendant une durée donnée
 * \param renderer le renderer à utiliser
 * \param image_path chemin de l'image de splash
 * \param duration_ms durée en millisecondes
 */
void show_splash_screen(SDL_Renderer *renderer, const char *image_path, int duration_ms) {
    SDL_Texture *splash = load_image(image_path, renderer);

    // Initialisation de l'audio pour le splash
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Mix_OpenAudio error: %s\n", Mix_GetError());
    }
    Mix_Chunk *splash_sound = Mix_LoadWAV("resources/splash_screen.wav");
    if (splash_sound) {
        Mix_PlayChannel(-1, splash_sound, 0);
    }

    if (splash) {
        clear_renderer(renderer);
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // plein écran
        SDL_RenderCopy(renderer, splash, NULL, &dest);
        update_screen(renderer);
        pause(duration_ms);
        clean_texture(splash);
    }

    if (splash_sound) {
        Mix_FreeChunk(splash_sound);
    }
    Mix_CloseAudio();
}

/**
 * \brief programme principal qui implémente la boucle du jeu
 */
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    world_t world;
    resources_t resources;
    SDL_Renderer *renderer;
    SDL_Window *window;
    Mix_Chunk *sound = NULL;

    // initialisation du jeu
    init(&window, &renderer, &resources);

    // Affichage de l'écran de démarrage
    show_splash_screen(renderer, "resources/splash_screen.png", 3000);

    init_data(&world);
    while (!world.gameover) { // tant que le jeu n'est pas fini
        // gestion des évènements
        handle_events(&world);

        // mise à jour des données liée à la physique du monde
        update_data(&world);

        // rafraichissement de l'écran
        draw_graphics(renderer, &world, &resources);

        Uint64 now_time = SDL_GetTicks64();
        Uint64 next_frame_time = world.last_frame_time + 1000 / MAX_FPS;
        if (next_frame_time > now_time) {
            pause(next_frame_time - now_time);
        }
    }

    // Jouer le son après fin du jeu
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    sound = world.has_won ? Mix_LoadWAV("resources/win_sound.wav") : Mix_LoadWAV("resources/loss_sound.wav");
    Mix_PlayChannel(-1, sound, 0);

    SDL_Event event;
    Uint32 end = SDL_GetTicks() + 3000;
    while (SDL_GetTicks() < end) {
        while (SDL_PollEvent(&event)) {
        }
    }

    // nettoyage final
    clean(window, renderer, &resources, &world);
    Mix_FreeChunk(sound);
    Mix_CloseAudio();

    return 0;
}
