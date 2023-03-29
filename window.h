#ifndef WINDOW_H
#define WINDOW_H

// #include <SFML/Transformable.hpp>
#include <SFML/Graphics.hpp>

const float RED_CONSTANT   =  8.5f;
const float GREEN_CONSTANT = 15.0f;
const float BLUE_CONSTANT  =  8.4f;

const sf::Color BLACK_COLOR = sf::Color::Black;
const sf::Color RED_COLOR = sf::Color::Red;

int print_mandelbrot (int window_width, float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_point_calculation, float r2_max);

int draw_mandelbrot_pixels (sf::RenderWindow *window, int window_width, float max_x_coordinate,
                                                      int window_height, float max_y_coordinate,
                                                      int max_n_point_calculation, float r2_max);
int print_pixel (sf::RenderWindow *window, int iteration, int max_n_point_calculation,
                                           int pixel_x, int pixel_y);

#endif /*WINDOW_H*/
