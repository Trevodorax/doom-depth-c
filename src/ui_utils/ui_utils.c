#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ui_utils.h"

rect_t * get_rectangle_grid(size_t nb_rectangles, rect_t *container) {
    if (nb_rectangles <= 0 || container == NULL) {
        return NULL;
    }

    int rows = (int)sqrt((int)nb_rectangles);
    int cols = ((int)nb_rectangles + rows - 1) / rows; // calculate columns based on rows

    // calculate width and height for each cell with a small margin
    int cell_width = ((int) container->w / cols) - 2;
    int cell_height = ((int) container->h / rows) - 2;

    rect_t *rect_grid = malloc(rows * cols * sizeof(rect_t));
    if (!rect_grid) {
        fprintf(stderr, "\nget_rectangle_grid error: couldn't allocate memory");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j; // calculate the index in the 1D array
            rect_grid[index].x = container->x + j * (cell_width + 2);
            rect_grid[index].y = container->y + i * (cell_height + 2);
            rect_grid[index].w = cell_width;
            rect_grid[index].h = cell_height;
        }
    }

    return rect_grid;
}

rect_t * get_rectangle_line(size_t nb_rectangles, rect_t *container, bool vertical) {
    if (nb_rectangles <= 0 || container == NULL) {
        return NULL;
    }

    int rows, cols;

    if (vertical) {
        rows = (int)nb_rectangles;
        cols = 1;
    } else {
        rows = 1;
        cols = (int)nb_rectangles;
    }

    int cell_width = ((int) container->w / cols) - 2;
    int cell_height = ((int) container->h / rows) - 2;

    rect_t *rect_display = malloc(rows * cols * sizeof(rect_t));
    if (!rect_display) {
        fprintf(stderr, "\nget_rectangle_display error: couldn't allocate memory");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            rect_display[index].x = container->x + j * (cell_width + 2);
            rect_display[index].y = container->y + i * (cell_height + 2);
            rect_display[index].w = cell_width;
            rect_display[index].h = cell_height;
        }
    }

    return rect_display;
}

rect_t * get_rectangle_layout(size_t nb_rectangles, rect_t * container, layout_t layout) {
    switch(layout) {
        case GRID:
            return get_rectangle_grid(nb_rectangles, container);
        case HORIZONTAL:
            return get_rectangle_line(nb_rectangles, container, false);
        case VERTICAL:
            return get_rectangle_line(nb_rectangles, container, true);
    }
}
