/**
 * \file resources.c
 * \brief Fichier contenant les implémentations des fonctions pour l'initialisation et la gestion des ressources du jeu
 * \author DANILKIN Artem, RUSCUK Emre
 * \version 1.0
 * \date 16 mai 2025
 */

#include "resources.h"
#include "utilities.h"

void init_mix() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Erreur en Mix_OpenAudio : %s\n", Mix_GetError());
    }
}

void clean_mix() {
    Mix_CloseAudio();
}

/**
 * \brief La fonction charge un son
 * \param exe_path le chemin de l'exécutable, utilisé pour construire le chemin complet du fichier
 * \param path le chemin du fichier correspondant au son
 * \return le son chargé
 */
Mix_Chunk *load_sound(const char *exe_path, const char *path) {
    char *full_path = exe_path != NULL ? concat_paths(exe_path, path) : strdup(path);
    Mix_Chunk *sound = Mix_LoadWAV(full_path);
    if (sound == NULL) {
        free(full_path);
        fprintf(stderr, "Erreur pendant chargement du son : %s\n", Mix_GetError());
        return NULL;
    }
    free(full_path);
    return sound;
}

/**
 * \brief Jouer un son
 * \param sound le son à nettoyer
 * \return le canal sur lequel le son est joué, ou -1 en cas d'erreur
 */
int play_sound(Mix_Chunk *sound) {
    return Mix_PlayChannel(-1, sound, 0);
}

/**
 * \brief Arrêter le son
 * \param sound_channel le canal sur lequel le son est joué
 */
void stop_sound(int sound_channel) {
    if (sound_channel != -1) {
        Mix_HaltChannel(sound_channel);
    }
}

/**
 * \brief La fonction nettoie un son
 * \param sound le son à nettoyer
 */
void clean_sound(Mix_Chunk *sound) {
    Mix_FreeChunk(sound);
}

/**
 * \brief La fonction initialise les ressources nécessaires à l'affichage graphique du jeu
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param renderer le renderer correspondant à l'écran de jeu
 * \param resources Les ressources du jeu
 */
void init_resources(const char *exe_dir, SDL_Renderer *renderer, resources_t *resources) {
    resources->splash_screen_texture = load_image(renderer, exe_dir, "resources/splash_screen.png", NULL, NULL);
    resources->background_texture = load_image(renderer, exe_dir, "resources/background.png", NULL, NULL);
    resources->spaceship_texture = load_image(renderer, exe_dir, "resources/spaceship.png", NULL, NULL);
    resources->finish_line_texture = load_image(renderer, exe_dir, "resources/finish_line.png", NULL, NULL);
    resources->meteorite_texture = load_image(renderer, exe_dir, "resources/meteorite.png", NULL, NULL);
    resources->splash_screen_sound = load_sound(exe_dir, "resources/splash_screen.wav");
    resources->loss_sound = load_sound(exe_dir, "resources/loss.wav");
    resources->win_sound = load_sound(exe_dir, "resources/win.wav");
    resources->font = NULL;
}

/**
 * \brief La fonction rafraichit la police. Peut être appelée plusieurs fois et utilisée pour changer la taille de la police
 * \param exe_dir le chemin de l'exécutable, utilisé pour charger les ressources
 * \param resources Les ressources du jeu
 * \param font_size La taille de la police
 */
void refresh_font(const char *exe_dir, resources_t *resources, int font_size) {
    if (resources->font != NULL) {
        clean_font(resources->font);
    }
    resources->font = load_font(exe_dir, "resources/COOPBL.ttf", font_size);
}

/**
 * \brief La fonction nettoie les ressources
 * \param resources Les ressources
 */
void clean_resources(resources_t *resources) {
    clean_texture(resources->splash_screen_texture);
    clean_texture(resources->background_texture);
    clean_texture(resources->spaceship_texture);
    clean_texture(resources->finish_line_texture);
    clean_texture(resources->meteorite_texture);
    clean_sound(resources->splash_screen_sound);
    clean_sound(resources->loss_sound);
    clean_sound(resources->win_sound);
    if (resources->font != NULL) {
        clean_font(resources->font);
    }
}
