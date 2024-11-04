#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "SDL.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif //DISPLAY_H
