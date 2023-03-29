#include "window.h"
#include <assert.h>

int print_mandelbrot (int window_width, float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_point_calculation, float r2_max)
{
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), header);

        while (window.isOpen()) {
                draw_mandelbrot_pixels(&window, window_width, max_x_coordinate,
                                                window_height, max_y_coordinate, max_n_point_calculation, r2_max);
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

        float r2 = 0;

        for (int pixel_y = 0; pixel_y < window_height; pixel_y++) {
                for (int pixel_x = 0; pixel_x < window_width; pixel_x++) {
                        y  = -max_y_coordinate + pixel_y * y_scale_coeff;
                        y0 = y;
                        x  = -max_x_coordinate + pixel_x * x_scale_coeff;
                        x0 = x;

                        int i = 0;
                        for (; i <= max_n_point_calculation; i++) {
                                x2 = x * x;
                                y2 = y * y;
                                xy = x * y;
                                r2 = x2 + y2;

                                if (x*x + y*y >= r2_max)
                                        break;

                                x = x2 - y2 + x0;
                                y = xy * 2 + y0;
                        }
                        print_pixel(window, i, max_n_point_calculation, pixel_x, pixel_y);
                }
        }
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
