#include <stdio.h>

#include "window.h"

int main ()
{
        int window_width           = 1600;
        float max_x_coordinate      = 2.f;
        int window_height           = 1200;
        float max_y_coordinate      = 1.5f;
        const char* header          = "Mandlebrot";
        int max_n_point_calculation = 255;
        float r2_max                = 4.f;
        return print_mandelbrot(window_width, max_x_coordinate,
                                window_height, max_y_coordinate,
                                header, max_n_point_calculation, r2_max);
}
