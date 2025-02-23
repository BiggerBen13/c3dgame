#include "r_draw.h"
#include "r_system.h"

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 720

struct g_state {
	render_ctx r_ctx;
	draw_ctx d_ctx;
	uint64_t dt;
} g_state;

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
	unsigned int return_value = 0;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_WINDOW_RESIZED:
			update_window_size(&g_state.r_ctx);
			break;
		case SDL_EVENT_QUIT:
			return_value += 1;
			break;
		case SDL_EVENT_KEY_DOWN:
			return_value += handle_keypress(&event);
			break;
		}
	}
	return return_value;
}

void frame(void)
{
	draw(&g_state.d_ctx, &g_state.r_ctx);
	SDL_GL_SwapWindow(g_state.r_ctx.window);
}

int main(void)
{
	init_render_context(&g_state.r_ctx);
	init_draw(&g_state.d_ctx);

	while (!update())
		frame();

	quit_render_systems(&g_state.r_ctx);
	return 0;
}
