#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

enum errors {
        NULL_CALLOC = 0x0CA110C,
        VBUF_ERR    = 0xBADB0F
};

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

struct pixel_t {
        sf::Color color  = BLACK_COLOR;
        int x = 0;
        int y = 0;
};

struct coordinates_t {
       float max_x  = 0.f;
       float max_y  = 0.f;
       int start_x  = 0  ;
       int start_y  = 0  ;
       float r2_max = 0.f;
};

int print_screen    (sf::RenderWindow *window, pixel_t *pixels, int window_height, int window_width);
int save_pixel      (int window_width, int iteration, int max_n_iteration, pixel_t *pixels, int pixel_x, int pixel_y);
int get_pressed_key ();
void change_scale (int pressed_button, coordinates_t *coords);

#endif /*GRAPHICS_H*/
