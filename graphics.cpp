#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>

#include "graphics.h"

int print_screen (sf::RenderWindow *window, pixel_t *pixels, int window_height, int window_width)
{
        assert(window);

        sf::Image mandel{};
        mandel.create(window_width, window_height, RED_COLOR);

        int n_pixels = window_height * window_width;

        for (int i = 0; i < n_pixels; i++) {
                mandel.setPixel(pixels[i].x, pixels[i].y, pixels[i].color);
        }
        sf::Texture texture{};
        texture.loadFromImage(mandel);

        sf::Sprite screen{};
        screen.setTexture(texture);

        window->draw(screen);

        return 0;
}

int save_pixel (int window_width, int iteration, int max_n_iteration, pixel_t *pixels, int pixel_x, int pixel_y)
{
        assert(pixels);

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

void change_scale (int pressed_button, coordinates_t *coords)
{
        switch (pressed_button) {
        case 0:
                return;
        case LEFT_BUTTON:
                coords->start_x -= 10;
                break;
        case RIGHT_BUTTON:
                coords->start_x += 10;
                break;
        case UP_BUTTON:
                coords->start_y -= 10;
                break;
        case DOWN_BUTTON:
                coords->start_y += 10;
                break;
        case ADD_BUTTON:
                coords->max_x *= 0.9f;
                coords->max_y *= 0.9f;
                break;
        case SUB_BUTTON:
                coords->max_x *= 1.1f;
                coords->max_y *= 1.1f;
                break;
        default:
                assert(0 && "UNKNOWN BUTTON\n");
        }

        return;
}
