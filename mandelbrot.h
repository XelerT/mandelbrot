#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "graphics.h"

int print_mandelbrot (int window_width,  float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_iteration, float r2_max);

int calc_mandelbrot_pixels (pixel_t *pixels, int window_width,        float max_x_coordinate,
                                             int window_height,       float max_y_coordinate,
                                             int *start_x_position,   int *start_y_position,
                                             int max_n_iteration, float r2_max);

#endif /*MANDELBROT_H*/
