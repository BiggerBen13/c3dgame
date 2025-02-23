#ifndef _R_SYSTEM_H
#define _R_SYSTEM_H

#include <SDL3/SDL.h>

/* Include sokol */

#define SOKOL_GLCORE
#include <sokol_gfx.h>

/* Include shaders */

/*** MACROS ***/

#define SDL_ERR_ASSERT(category, expr)                                     \
	if (!(expr)) {                                                     \
		SDL_LogError((category), "SDL_ERROR: %s", SDL_GetError()); \
		return -1;                                                 \
	}

/*** STRUCTS ***/

typedef struct {
	SDL_Window *window;

/* here fore eventual alternative rendering backend */
#if defined(SOKOL_GLCORE) || defined(SOKOL_GLES)
	SDL_GLContext gl_ctx;
#endif

        int window_width;
        int window_height;

} render_ctx;

/*** FUNCTIONS ***/

 int init_render_context(render_ctx *ctx);

 int quit_render_systems(render_ctx *ctx);

 sg_swapchain sdl_swapchain(render_ctx *ctx);

 void update_window_size(render_ctx *ctx);

#endif
