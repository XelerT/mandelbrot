#include "window.h"
#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>

int print_mandelbrot (int window_width, float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_iteration, float r2_max)
{
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), header);

        float start_x_position  = 0.f;
        float start_y_position  = 0.f;

        sf::Text fps_counter;
        sf::Clock clock;
        sf::Time current_time = clock.getElapsedTime();
        sf::Time previous_time = current_time;

        while (window.isOpen()) {
                draw_mandelbrot_pixels(&window, window_width, max_x_coordinate,
                                                window_height, max_y_coordinate,
                                                &start_x_position, &start_y_position,
                                                max_n_iteration, r2_max);
                window.display();

                sf::Event event;
                while (window.pollEvent(event))
                        if (event.type == sf::Event::Closed)
                                window.close();

                current_time = clock.getElapsedTime();
                double fps = 1.0f / (current_time.asSeconds() - previous_time.asSeconds());
                printf("curr = %lf; prev = %lf; fps = %lf\n", current_time.asSeconds(), previous_time.asSeconds(), fps);
                previous_time = current_time;
        }
        return 0;
}

int int2string (int number, char *dest, int max_length)
{
        assert(dest);

        for (int i = max_length - 1; i >= 0; i--) {
                dest[i] = (char) number % 10 + '0';
                number /= 10;
        }

        return 0;
}

int draw_mandelbrot_pixels (sf::RenderWindow *window, int window_width, float max_x_coordinate,
                                                      int window_height, float max_y_coordinate,
                                                      float *start_x_position, float *start_y_position,
                                                      int max_n_iteration, float r2_max)
{
        assert(window);

        float x_scale_coeff = 2 * max_x_coordinate / (float) window_width;
        float y_scale_coeff = 2 * max_y_coordinate / (float) window_height;

        __m256 x = _mm256_set1_ps(-max_x_coordinate);
        __m256 y = _mm256_set1_ps(-max_y_coordinate);

        __m256 x0 = {};
        __m256 y0 = {};

        __m256 avx_max_x_coord = x;

        // __m256 _mm256_set1_ps (float a)
        // __m256 _mm256_add_ps (__m256 a, __m256 b)
        // __m256 _mm256_mul_ps (__m256 a, __m256 b)
        // __m256i _mm256_sub_epi64 (__m256i a, __m256i b)

        __m256 avx_r2_max = _mm256_set1_ps(r2_max);

        // change_scale(get_pressed_key(), start_x_position, start_y_position, &x_scale_coeff, &y_scale_coeff);
        window_height += (int) *start_y_position;
        window_width  += (int) *start_x_position;

        __m256 two = _mm256_set_ps(2, 2, 2, 2, 2, 2, 2, 2);
        __m256 _76543210 = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);

        __m256 dx_coeffs = _mm256_mul_ps(_76543210, _mm256_set1_ps(x_scale_coeff));

        for (int pixel_y = (int) *start_y_position; pixel_y < window_height; pixel_y++) {
                // y = pixel_y * y_scale_coeff + -max_y_coordinate
                y = _mm256_set1_ps((float) pixel_y * y_scale_coeff - max_y_coordinate);
                y0 = y;
                for (int pixel_x = (int) *start_x_position; pixel_x < window_width; pixel_x += 8) {
                        y = y0;
                        __m256 scaled_pixel_x = _mm256_set1_ps((float) pixel_x * x_scale_coeff);
                        // x = pixel_x * x_scale_coeff + -max_x_coordinate
                        x = _mm256_add_ps(_mm256_add_ps(scaled_pixel_x, dx_coeffs), avx_max_x_coord);
                        x0 = x;
                        __m256i N = _mm256_setzero_si256();

                        for (int i = 0; i < max_n_iteration; i++) {
                                __m256 x2 = _mm256_mul_ps(x, x);
                                __m256 y2 = _mm256_mul_ps(y, y);
                                __m256 xy = _mm256_mul_ps(x, y);

                                __m256 r2 = _mm256_add_ps(x2, y2);

                                __m256 cmp = _mm256_cmp_ps(r2, avx_r2_max, _CMP_LT_OQ);
                                if (!_mm256_movemask_ps(cmp)) {
                                        break;
                                }
                                N = _mm256_sub_epi32(N, _mm256_castps_si256(cmp));

                                x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0);
                                y = _mm256_add_ps(_mm256_mul_ps(two, xy), y0);
                        }

                        int *n_iter = (int*) &N;
                        for (int i = 0; i < 8; i++) {
                                print_pixel(window, *(n_iter + i), max_n_iteration, pixel_x + i, pixel_y);
                        }
                }
        }

        return 0;
}

int print_pixel (sf::RenderWindow *window, int iteration, int max_n_iteration, int pixel_x, int pixel_y)
{
        assert(window);

        sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
        sf::Color color{};

        if (iteration >= max_n_iteration) {
                color = BLACK_COLOR;
        } else {
                float factor = (float) iteration / (float) max_n_iteration;

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
