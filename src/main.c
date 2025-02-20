#include <SDL3/SDL.h>
#include <stdatomic.h>
#include <stdio.h>
#include "types.h"
#include <string.h>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 720

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
} context;

int handle_keypress(SDL_Event *event)
{
	switch (event->key.key) {
	case SDLK_ESCAPE:
		return 1;
	default:
		return 0;
	}
}

int update(void)
{
	SDL_Event event;
	u32 return_value = 0;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_EVENT_QUIT:
		return_value += 1;
	case SDL_EVENT_KEY_DOWN:
		return_value += handle_keypress(&event);
	}
	return return_value;
}

int draw(context *ctx)
{
        SDL_RenderClear(ctx->renderer);
        SDL_RenderPresent(ctx->renderer);
        return 0;
}

int init(context *ctx)
{
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		fprintf(stderr, "UNABLE TO INIT SDL: %s", SDL_GetError());
		return -1;
	}

	ctx->window = SDL_CreateWindow("WINDOW", WINDOW_HEIGHT, WINDOW_WIDTH,
				       SDL_WINDOW_RESIZABLE);
	if (ctx->window == NULL) {
		SDL_Log("UNABLE TO OPEN WINODW: %s", SDL_GetError());
		return -1;
	}

        ctx->renderer = SDL_CreateRenderer(ctx->window, NULL);

	if (ctx->renderer == NULL) {
		SDL_Log("UNABLE TO CREATE RENDERER: %s", SDL_GetError());
		return -1;
	}

        SDL_SetRenderDrawColor(ctx->renderer, 0xff, 0xff, 0xff, 0xff);


	return 0;
}

int quit(context *ctx)
{
	SDL_DestroyRenderer(ctx->renderer);
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
	return 0;
}

int main(void)
{
	context ctx;

	init(&ctx);

	while (!update() && !draw(&ctx))
		;

	return 0;
	quit(&ctx);
}
