#ifndef __SDL2_TTF_LIGHT__H
#define __SDL2_TTF_LIGHT__H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void init_ttf();

TTF_Font *load_font(const char *exe_path, const char *path, int font_size);

void draw_text(SDL_Renderer *renderer, double x, double y, bool center, TTF_Font *font, const char *text);

void clean_font(TTF_Font *font);

void clean_ttf();

#endif