#include <stdio.h>
#include <string.h>	

#include "array.h"
#include "mesh.h"

mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = {0, 0, 0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
		{.x = -1, .y = -1, .z = -1},
		{.x = -1, .y = 1, .z = -1},
		{.x = 1, .y = 1, .z = -1},
		{.x = 1, .y = -1, .z = -1},
		{.x = 1, .y = 1, .z = 1},
		{.x = 1, .y = -1, .z = 1},
		{.x = -1, .y = 1, .z = 1},
		{.x = -1, .y = -1, .z = 1}
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{.a = 1, .b = 2, .c = 3},
	{.a = 1, .b = 3, .c = 4},
	// right
	{.a = 4, .b = 3, .c = 5},
	{.a = 4, .b = 5, .c = 6},
	// back
	{.a = 6, .b = 5, .c = 7},
	{.a = 6, .b = 7, .c = 8},
	//left
	{.a = 8, .b = 7, .c = 2},
	{.a = 8, .b = 2, .c = 1},
	// top
	{.a = 2, .b = 7, .c = 5},
	{.a = 2, .b = 5, .c = 3},
	// bottom
	{.a = 6, .b = 8, .c = 1},
	{.a = 6, .b = 1, .c = 4},
};

void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}

	for (int i = 0; i < N_CUBE_FACES; i++) {
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

void load_obj_file_data(char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("ERROR: Can't load file at: %s\n", filename);
		return 1;
	}

	printf("SUCESS: Loaded obj file at: %s\n", filename);

	char line[512];
	while (fgets(line, sizeof(line), file)) {
		if(strncmp(line, "v ", 2) == 0) {
			vec3_t vtx;
			sscanf(line, "v %f %f %f", &vtx.x, &vtx.y, &vtx.z);
			array_push(mesh.vertices, vtx);
		}

		if (strncmp(line, "f ", 2) == 0) {
			int vtx_indices[3], texture_indices[3], normal_indices[3];

			int matches = sscanf(
				line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vtx_indices[0], &texture_indices[0], &normal_indices[0],
				&vtx_indices[1], &texture_indices[1], &normal_indices[1],
				&vtx_indices[2], &texture_indices[2], &normal_indices[2]
			);

			if (matches = !9) {
				printf("ERROR: Malformed face data %s\n", line);
				continue;
			}

			face_t face = {
				.a = vtx_indices[0],
				.b = vtx_indices[1],
				.c = vtx_indices[2]
			};
			array_push(mesh.faces, face);
		}
	}


	fclose(file);
}