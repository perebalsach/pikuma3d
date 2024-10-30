#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING != 0)) {
		fprintf(stderr, "Error initializintg SDL\n");
		return false;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS
	);
	
	if (!window) {
		fprintf(stderr, "Error creating SDL Window\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating the Renderer\n");
		return false;
	}
}

void update() {

}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

}

void render() {

}

void setup() {

}

int main(int argc, char* argv[]) {
	is_running = initialize_window();

	setup();
	while (is_running) {
		process_input();
		update();
		render();
	}

	return 0;
}