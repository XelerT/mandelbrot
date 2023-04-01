#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "graphics.h"

enum performance {
        AVX_PERFORMANCE,
        MID_PERFORMANCE,
        NO_PERFORMANCE
};

int print_mandelbrot (int window_width, int window_height, coordinates_t *coords,
                                        const char* header, int max_n_iteration);

int calc_mandelbrot_pixels (pixel_t *pixels, int window_width, int window_height, int max_n_iteration,
                                                coordinates_t *coords);
int mid_calc_mandelbrot_pixels (pixel_t *pixels, int window_width, int window_height, int max_n_iteration,
                                                coordinates_t *coords);
int slow_calc_mandelbrot_pixels (pixel_t *pixels, int window_width, int window_height, int max_n_iteration,
                                                coordinates_t *coords);


#endif /*MANDELBROT_H*/
