#include "defs.h"

/*
 * This is here to create the Sokol Implementations, if it were in the header
 * file we would get multiple linkage issues 
 */
#define SOKOL_IMPL

#include "r_system.h"

extern sg_swapchain sdl_swapchain(render_ctx *ctx)
{
	sg_swapchain swapchain = { .width = ctx->window_width,
				   .height = ctx->window_height,
				   .color_format = SG_PIXELFORMAT_RGBA8,
				   .sample_count = 1,
				   .gl.framebuffer = 0 };
	return swapchain;
}

int init_render_context(render_ctx *ctx)
{
	SDL_ERR_ASSERT(SDL_LOG_CATEGORY_SYSTEM,
		       SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS))

	/*
        * Ensure that OPENGL context is created with correct attributes for the
        * sokol backend
        */
#ifdef SOKOL_GLES3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			    SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
#ifdef SOKOL_GLCORE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			    SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

	ctx->window = SDL_CreateWindow("WINDOW", WINDOW_HEIGHT, WINDOW_WIDTH,
				       SDL_WINDOW_RESIZABLE
#if defined(SOKOL_GLCORE) || defined(SOKOL_GLES3)
					       | SDL_WINDOW_OPENGL
#endif
	);

	SDL_ERR_ASSERT(SDL_LOG_CATEGORY_VIDEO, ctx->window != NULL)

	ctx->window_height = WINDOW_HEIGHT;
	ctx->window_width = WINDOW_WIDTH;

#if defined(SOKOL_GLCORE) || defined(SOKOL_GLES3)
	ctx->gl_ctx = SDL_GL_CreateContext(ctx->window);

	SDL_ERR_ASSERT(SDL_LOG_CATEGORY_VIDEO, ctx->gl_ctx != NULL)
#endif

	sg_setup(&(sg_desc){
		.environment.defaults = { .color_format = SG_PIXELFORMAT_RGBA8,
					  .depth_format = SG_PIXELFORMAT_NONE,
					  .sample_count = 1 },
	});

	return 0;
}

int quit_render_systems(render_ctx *ctx)
{
	sg_shutdown();

#if defined(SOKOL_GLCORE) || defined(SOKOL_GLES3)
	SDL_GL_DestroyContext(ctx->gl_ctx);
#endif
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
	return 0;
}

void update_window_size(render_ctx *ctx)
{
	SDL_GetWindowSize(ctx->window, &ctx->window_width, &ctx->window_height);
}
