#include <stdio.h>

#include "mandelbrot.h"
#include "graphics.h"

int main ()
{
        int window_width            = 1600;
        int window_height           = 1200;
        const char* header          = "Mandlebrot";
        coordinates_t coords        = {
                .max_x   =  2.f,
                .max_y   = 1.5f,
                .start_x =    0,
                .start_y =    0,
                .r2_max  =  4.f
        };
        int max_n_iteration         = 255;
        return print_mandelbrot(window_width, window_height, &coords, header, max_n_iteration);
}
