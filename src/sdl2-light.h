#ifndef __SDL2_LIGHT__H__
#define __SDL2_LIGHT__H__

#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

void clean_sdl(SDL_Renderer *renderer, SDL_Window *window);

SDL_Texture *load_image(SDL_Renderer *renderer, const char *exe_path, const char *path, SDL_Surface **image_surface_out, bool *success);

int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int width, int height);

Uint32 get_pixel_RGBA32(SDL_Surface *surface, size_t x, size_t y);

void clean_texture(SDL_Texture *texture);

void clear_renderer(SDL_Renderer *renderer);

void update_screen(SDL_Renderer *renderer);

#endif
