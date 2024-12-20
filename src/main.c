#include <stdint.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"


triangle_t* triangles_to_render = NULL;
bool is_running = false;
int previous_frame_time = 0;

vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };
float fov_factor = 640;


void setup(void) {
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			window_width,
			window_height
	);

	 load_cube_mesh_data();
	//if (load_obj_file_data("./res/meshes/cube.obj") != 1) {
	//	printf("ERROR: Failed to load obj file data.\n");
	//	exit(1);
	//}
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		if (event.key.keysym.sym == SDLK_1)
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4)
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_c)
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			cull_method = CULL_NONE;
		break;
	}
}

vec2_t project(vec3_t point) {
	vec2_t projected_point = {
			.x = (fov_factor * point.x) / point.z,
			.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void) {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();

	triangles_to_render = NULL;

	mesh.rotation.x += (float)0.01;
	mesh.rotation.y += (float)0.01;
	mesh.rotation.z += (float)0.01;

	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t faces_vertices[3];
		faces_vertices[0] = mesh.vertices[mesh_face.a - 1];
		faces_vertices[1] = mesh.vertices[mesh_face.b - 1];
		faces_vertices[2] = mesh.vertices[mesh_face.c - 1];


		vec3_t transformed_vertices[3];

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = faces_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}

		if (cull_method == CULL_BACKFACE) {
			vec3_t vector_a = transformed_vertices[0];
			vec3_t vector_b = transformed_vertices[1];
			vec3_t vector_c = transformed_vertices[2];

			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);


			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);

			vec3_t camera_ray = vec3_sub(camera_position, vector_a);

			float normal_camera = vec3_dot(normal, camera_ray);

			if (normal_camera < 0) {
				continue;
			}
		}

		vec2_t projected_points[3];

		// Loop all the vertices to perform projection
		for (int j = 0; j < 3; j++) {
			projected_points[j] = project(transformed_vertices[j]);

			projected_points[j].x += (float)(window_width / 2);
			projected_points[j].y += (float)(window_height / 2);
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		triangle_t projected_triangle = {
			.points = {
				{projected_points[0].x, projected_points[0].y},
				{projected_points[1].x, projected_points[1].y},
				{projected_points[2].x, projected_points[2].y},
			},
			.color = mesh_face.color,
			.avg_depth = avg_depth 
		};

		array_push(triangles_to_render, projected_triangle);
	}

	// Sort the faces based on the depth
	int size = sizeof(triangles_to_render) / sizeof(triangles_to_render[0]);
	qsort(&triangles_to_render, size);
}

void render(void) {

	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];

		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_filled_triangle(
					(int)triangle.points[0].x, (int)triangle.points[0].y,
					(int)triangle.points[1].x, (int)triangle.points[1].y,
					(int)triangle.points[2].x, (int)triangle.points[2].y,
					triangle.color
				);
		}
		
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_triangle(
					(int)triangle.points[0].x, (int)triangle.points[0].y,
					(int)triangle.points[1].x, (int)triangle.points[1].y,
					(int)triangle.points[2].x, (int)triangle.points[2].y,
					triangle.color);
		}

		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect((int)triangle.points[0].x - 3, (int)triangle.points[0].y -3, 6, 6, 0xFFFF0000);
			draw_rect((int)triangle.points[1].x - 3, (int)triangle.points[1].y -3, 6, 6, 0xFFFF0000);
			draw_rect((int)triangle.points[2].x - 3, (int)triangle.points[2].y -3, 6, 6, 0xFFFF0000);
		}
	}


	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0x808080);
	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}


int main(void) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}
