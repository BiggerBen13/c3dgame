#ifndef _R_DRAW_H
#define _R_DRAW_H

#include "r_system.h"

#include "shader/triangle.glsl.h"

#include <stdlib.h>
#include <string.h>

#define NUM_BUFFERS 1
#define BUFFER_SIZE 4096

typedef struct {
	/*sg_shader shaders[NUM_SHADERS];*/
	sg_pipeline pipeline;
	sg_bindings bindings;
        sg_pass_action pass_action;
	void *buffer[NUM_BUFFERS];
} draw_ctx;

void init_draw(draw_ctx *d_ctx);

void draw(draw_ctx *d_ctx, render_ctx *r_ctx);

#endif
