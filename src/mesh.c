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
	{.a = 1, .b = 2, .c = 3, .color = 0xFFFF0000 },
	{.a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
	// right
	{.a = 4, .b = 3, .c = 5, .color = 0xFF00FF00 },
	{.a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
	// back
	{.a = 6, .b = 5, .c = 7, .color = 0xFF0000FF },
	{.a = 6, .b = 7, .c = 8, .color = 0xFF0000FF },
	//left
	{.a = 8, .b = 7, .c = 2, .color = 0xFFFFFF00 },
	{.a = 8, .b = 2, .c = 1, .color = 0xFFFFFF00 },
	// top
	{.a = 2, .b = 7, .c = 5, .color = 0xFFFF00FF },
	{.a = 2, .b = 5, .c = 3, .color = 0xFFFF00FF },
	// bottom
	{.a = 6, .b = 8, .c = 1, .color = 0xFF00FFFF },
	{.a = 6, .b = 1, .c = 4, .color = 0xFF00FFFF },
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

int load_obj_file_data(char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("ERROR: Can't load file at: %s\n", filename);
		return 0;
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
				printf("ERROR: Malformed face data in obj file %s\n", line);
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
	return 1;
}