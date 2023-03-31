#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>

#include "graphics.h"

int print_screen (sf::RenderWindow *window, pixel_t *pixels, int window_height, int window_width)
{
        assert(window);

        sf::RectangleShape pixel_rectangle(sf::Vector2f(1.f, 1.f));
        sf::Color pixel_color{};

        int n_pixels = window_height * window_width;

        for (int i = 0; i < n_pixels; i++) {
                pixel_color = pixels[i].color;
                pixel_rectangle.setFillColor(pixel_color);

                pixel_rectangle.setPosition((float) pixels[i].x, (float) pixels[i].y);
                window->draw(pixel_rectangle);
        }
        return 0;
}

int save_pixel (int window_width, int iteration, int max_n_iteration, pixel_t *pixels, int pixel_x, int pixel_y)
{
        sf::Color pixel_color{};

        if (iteration < max_n_iteration) {
                float factor = (float) iteration / (float) max_n_iteration;

                pixel_color.r = (sf::Uint8) (RED_CONSTANT   * factor * factor       * factor       * (1 - factor) * 255.f);
                pixel_color.g = (sf::Uint8) (GREEN_CONSTANT * factor * factor       * (1 - factor) * (1 - factor) * 255.f);
                pixel_color.b = (sf::Uint8) (BLUE_CONSTANT  * factor * (1 - factor) * (1 - factor) * (1 - factor) * 255.f);
        }

        int pixel_number = pixel_y * window_width + pixel_x;
        pixels[pixel_number].color = pixel_color;
        pixels[pixel_number].x = pixel_x;
        pixels[pixel_number].y = pixel_y;

        return 0;
}

#define check_button(stdn_name, my_name)         if (sf::Keyboard::isKeyPressed(sf::Keyboard::stdn_name))    \
                                                        return my_name;

int get_pressed_key ()
{
        check_button(Left,    LEFT_BUTTON);
        check_button(Right,  RIGHT_BUTTON);
        check_button(Up,        UP_BUTTON);
        check_button(Down,    DOWN_BUTTON);
        check_button(Add,      ADD_BUTTON);
        check_button(Subtract, SUB_BUTTON);

        return 0;
}
#undef check_button

void change_scale (int pressed_button, int *x, int *y, float *x_scale, float *y_scale)
{
        switch (pressed_button) {
        case 0:
                return;
        case LEFT_BUTTON:
                *x -= 10;
                break;
        case RIGHT_BUTTON:
                *x += 10;
                break;
        case UP_BUTTON:
                *y -= 10;
                break;
        case DOWN_BUTTON:
                *y += 10;
                break;
        case ADD_BUTTON:
                *x_scale *= 0.9f;
                *y_scale *= 0.9f;
                break;
        case SUB_BUTTON:
                *x_scale *= 1.1f;
                *y_scale *= 1.1f;
                break;
        default:
                assert(0 && "UNKNOWN BUTTON\n");
        }

        return;
}
