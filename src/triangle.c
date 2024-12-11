#include "triangle.h"
#include "display.h"


void int_swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int *array, int low, int high) {
	int pivot = array[high];
	int i = low - 1; // Index of smaller element

	for (int j = low; j < high; j++) {
		if (array[j] < pivot) {
			i++;
			int_swap(&array[i], &array[j]);
		}
	}
	int_swap(&array[i + 1], &array[high]);
	return i + 1;
}

void quick_sort_recursive(int *array, int low, int high) {
	if (low < high) {
		int pi = partition(array, low, high);

		// Recursively sort elements before and after partition
		quick_sort_recursive(array, low, pi - 1);
		quick_sort_recursive(array, pi + 1, high);
	}
}

void qsort(int *array, int size) {
	if (size > 1) {
		quick_sort_recursive(array, 0, size - 1);
	}
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// Slope calculation
	float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

	float x_start = x0;
	float x_end = x0;

	for (int y = y0; y <= y2; y++) {
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// Slope calculation
	float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
	float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

	float x_start = x2;
	float x_end = x2;

	for (int y = y2; y >= y0; y--) {
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// Sort vertices
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
	int My = y1;
	int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

	// Draw triangles
	fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
	fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
	}
}

