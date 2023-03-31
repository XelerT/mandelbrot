#ifndef WINDOW_H
#define WINDOW_H

// #include <SFML/Transformable.hpp>
#include <SFML/Graphics.hpp>

const float RED_CONSTANT   =  8.5f;
const float GREEN_CONSTANT = 15.0f;
const float BLUE_CONSTANT  =  8.4f;

enum buttons {
        LEFT_BUTTON   = 0x1EF1,
        RIGHT_BUTTON  = 0x4133,
        UP_BUTTON     = 0x4213,
        DOWN_BUTTON   = 0xD014,
        ADD_BUTTON    = 0xADDB,
        SUB_BUTTON    = 0x20BB
};

const sf::Color BLACK_COLOR = sf::Color::Black;
const sf::Color RED_COLOR = sf::Color::Red;

int print_mandelbrot (int window_width, float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_point_calculation, float r2_max);

int draw_mandelbrot_pixels (sf::RenderWindow *window, int window_width, float max_x_coordinate,
                                                      int window_height, float max_y_coordinate,
                                                      float *start_x_position, float *start_y_position,
                                                      int max_n_point_calculation, float r2_max);

int print_pixel (sf::RenderWindow *window, int iteration, int max_n_point_calculation,
                                           int pixel_x, int pixel_y);
int get_pressed_key ();
void change_scale (int pressed_button, float *x, float *y, float *x_scale, float *y_scale);

int int2string (int number, char *dest, int max_length);

#endif /*WINDOW_H*/
