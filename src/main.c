#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "audio.h"
#include "main.h"
#include "rand.h"

/* Function prototypes */
void	init_disp(struct game *cur_game);
void	kill_disp(struct game *cur_game);

int
main()
{
	SDL_Event event;
	struct game cur_game = {
		{1280,			/* display.w */
		720,			/* display.h */
		"namegoeshere",		/* display.name */
		NULL,			/* display.window */
		NULL},			/* display.renderer */
		SDL_FALSE		/* running */
	};
	
	/* Initialize SDL, seed rng, and initialize display */
	SDL_Init(SDL_INIT_EVERYTHING);
	seed_rng();
	init_disp(&cur_game);
	init_audio();
	/* Enter main loop */
	while (cur_game.running == SDL_TRUE) {
		/* Wait for an event */
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT) {
			cur_game.running = SDL_FALSE;
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				cur_game.running = SDL_FALSE;
				break;
			case SDLK_SPACE:
				swap_songs();
				break;
			case SDLK_RETURN:
				play_sound(SDL_TRUE);
				break;
			default:
				play_sound(SDL_FALSE);
				break;
			}
		}
	}
	/* Destroy display and exit */
	kill_disp(&cur_game);
	kill_audio();
	SDL_Quit();
	return 0;
}

void
init_disp(struct game *cur_game)
{
	SDL_Event event;
	
	cur_game->display.window = SDL_CreateWindow(cur_game->display.name, 0, 0, cur_game->display.w, cur_game->display.h, SDL_WINDOW_SHOWN);
	cur_game->display.renderer = SDL_CreateRenderer(cur_game->display.window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE|SDL_RENDERER_PRESENTVSYNC);
	// You may want this
	//SDL_SetRenderDrawBlendMode(cur_game->display.renderer, SDL_BLENDMODE_BLEND);
	/* Create output texture */
	/* Poll for an event and wait 100ms - bring the window to the foreground */
	SDL_PollEvent(&event);
	SDL_Delay(100);
	/* Game is running */
	cur_game->running = SDL_TRUE;
}

void
kill_disp(struct game *cur_game)
{
	/* Delay to prevent segfaults in other thread loops */
	SDL_Delay(100);
	SDL_DestroyRenderer(cur_game->display.renderer);
	SDL_DestroyWindow(cur_game->display.window);
}
