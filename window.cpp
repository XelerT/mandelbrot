#include "window.h"
#include <assert.h>

int print_mandelbrot (int window_width, float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_point_calculation, float r2_max)
{
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), header);

        float start_x_position  = 0.f;
        float start_y_position  = 0.f;

        while (window.isOpen()) {
                draw_mandelbrot_pixels(&window, window_width, max_x_coordinate,
                                                window_height, max_y_coordinate,
                                                &start_x_position, &start_y_position,
                                                max_n_point_calculation, r2_max);
                window.display();

                sf::Event event;
                while (window.pollEvent(event))
                        if (event.type == sf::Event::Closed)
                                window.close();
        }
        return 0;
}

int draw_mandelbrot_pixels (sf::RenderWindow *window, int window_width, float max_x_coordinate,
                                                      int window_height, float max_y_coordinate,
                                                      float *start_x_position, float *start_y_position,
                                                      int max_n_point_calculation, float r2_max)
{
        assert(window);

        float x_scale_coeff = 2 * max_x_coordinate / (float) window_width;
        float y_scale_coeff = 2 * max_y_coordinate / (float) window_height;

        float x = -max_x_coordinate;
        float y = -max_y_coordinate;

        float x0 = 0;
        float y0 = 0;

        float x2 = 0;
        float y2 = 0;
        float xy = 0;

        // change_scale(get_pressed_key(), start_x_position, start_y_position, &x_scale_coeff, &y_scale_coeff);
        window_height += (int) *start_y_position;
        window_width += (int) *start_x_position;

        for (int pixel_y = (int) *start_y_position; pixel_y < window_height; pixel_y++) {
                for (int pixel_x = (int) *start_x_position; pixel_x < window_width; pixel_x++) {
                        y  = -max_y_coordinate + (float) pixel_y * y_scale_coeff;
                        y0 = y;
                        x  = -max_x_coordinate + (float) pixel_x * x_scale_coeff;
                        x0 = x;



                        int i = 0;
                        for (; i <= max_n_point_calculation; i++) {
                                x2 = x * x;
                                y2 = y * y;
                                xy = x * y;

                                if (x2 + y2 >= r2_max)
                                        break;

                                x = x2 - y2 + x0;
                                y = xy * 2  + y0;
                        }
                        print_pixel(window, i, max_n_point_calculation, pixel_x, pixel_y);
                }
                // window->pollEvent(event);
        }

        // if (event.type ==  sf::Event::KeyPressed) {
        // }

        return 0;
}

int print_pixel (sf::RenderWindow *window, int iteration, int max_n_point_calculation, int pixel_x, int pixel_y)
{
        assert(window);

        sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
        sf::Color color{};

        if (iteration >= max_n_point_calculation) {
                color = BLACK_COLOR;
        } else {
                float factor = (float) iteration / (float) max_n_point_calculation;

                color.r = (sf::Uint8) (RED_CONSTANT * factor * factor * factor * (1 - factor) * 255.f);
                color.g = (sf::Uint8) (GREEN_CONSTANT * factor * factor * (1 - factor) * (1 - factor) * 255.f);
                color.b = (sf::Uint8) (BLUE_CONSTANT * factor * (1 - factor) * (1 - factor) * (1 - factor) * 255.f);
        }

        rectangle.setPosition((float) pixel_x, (float) pixel_y);
        rectangle.setFillColor(color);

        window->draw(rectangle);

        return 0;
}

#define check_button(stdn_name, my_name)         if (sf::Keyboard::isKeyPressed(sf::Keyboard::stdn_name))    \
                                                        return my_name;

int get_pressed_key ()
{
        check_button(Left,      LEFT_BUTTON);
        check_button(Right,     RIGHT_BUTTON);
        check_button(Up,        UP_BUTTON);
        check_button(Down,      DOWN_BUTTON);
        check_button(Add,       ADD_BUTTON);
        check_button(Subtract, SUB_BUTTON);

        return 0;
}
#undef check_button

void change_scale (int pressed_button, float *x, float *y, float *x_scale, float *y_scale)
{
        switch (pressed_button) {
        case 0:
                return;
        case LEFT_BUTTON:
                *x -= 10.f;
                printf("left");
                break;
        case RIGHT_BUTTON:
                *x += 10.f;
                printf("right");
                break;
        case UP_BUTTON:
                *y -= 10.f;
                printf("up\n");
                break;
        case DOWN_BUTTON:
                printf("down\n");
                *y += 10.f;
                break;
        case ADD_BUTTON:
                printf("add\n");
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
