#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ui_utils.h"

rect_t * get_rectangle_grid(int nb_rectangles, rect_t *container) {
    if (nb_rectangles <= 0 || container == NULL) {
        return NULL;
    }

    int rows = (int)sqrt(nb_rectangles);
    int cols = (nb_rectangles + rows - 1) / rows; // calculate columns based on rows

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

rect_t *get_rectangle_line(int nb_rectangles, rect_t *container, bool vertical, int margin) {
    if (nb_rectangles <= 0 || container == NULL) {
        return NULL;
    }

    int rows, cols;

    if (vertical) {
        rows = nb_rectangles;
        cols = 1;
    } else {
        rows = 1;
        cols = nb_rectangles;
    }

    int cell_width = ((int) container->w / cols) - margin;
    int cell_height = ((int) container->h / rows) - margin;

    rect_t *rect_display = malloc(rows * cols * sizeof(rect_t));
    if (!rect_display) {
        fprintf(stderr, "\nget_rectangle_display error: couldn't allocate memory");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            rect_display[index].x = container->x + j * (cell_width + margin);
            rect_display[index].y = container->y + i * (cell_height + margin);
            rect_display[index].w = cell_width;
            rect_display[index].h = cell_height;
        }
    }

    return rect_display;
}

rect_t *get_rectangle_layout(int nb_rectangles, rect_t *container, layout_t layout, int margin) {
    switch(layout) {
        case GRID:
            return get_rectangle_grid(nb_rectangles, container);
        case HORIZONTAL:
            return get_rectangle_line(nb_rectangles, container, false, margin);
        case VERTICAL:
            return get_rectangle_line(nb_rectangles, container, true, margin);
    }
}
