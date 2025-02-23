#define SOKOL_SHDC_IMPL

#include "r_draw.h"

#include "r_types.h"


void init_draw(draw_ctx *d_ctx)
{
	// a vertex buffer with 3 vertices
	vertex verts[] = {
		// positions            // colors
		{ { 0.0f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
	};

	d_ctx->bindings.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
		.data = SG_RANGE(verts), .label = "triangle-vertices" });

	// create shader from code-generated sg_shader_desc
	sg_shader shd =
		sg_make_shader(triangle_shader_desc(sg_query_backend()));

	// create a pipeline object
	d_ctx->pipeline = sg_make_pipeline(&(sg_pipeline_desc){
		.shader = shd,

		// if the vertex layout doesn't have gaps, don't need to provide strides and offsets
		.layout = { .attrs = { [ATTR_triangle_position].format =
					       SG_VERTEXFORMAT_FLOAT3,
				       [ATTR_triangle_color0].format =
					       SG_VERTEXFORMAT_FLOAT4 } },
		.label = "triangle-pipeline" });

	// a pass action to clear framebuffer to black
	d_ctx->pass_action = (sg_pass_action){
		.colors[0] = { .load_action = SG_LOADACTION_CLEAR,
			       .clear_value = { 0.0f, 0.0f, 0.0f, 1.0f } }
	};
}

void draw(draw_ctx *d_ctx, render_ctx *r_ctx)
{
	sg_begin_pass(&(sg_pass){ .action = d_ctx->pass_action,
				  .swapchain = sdl_swapchain(r_ctx) });
	sg_apply_pipeline(d_ctx->pipeline);
	sg_apply_bindings(&d_ctx->bindings);
	sg_draw(0, 3, 1);
	sg_end_pass();
	sg_commit();
}
