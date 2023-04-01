#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>

#include "graphics.h"
#include "mandelbrot.h"

int print_mandelbrot (int window_width,  float max_x_coordinate,
                      int window_height, float max_y_coordinate,
                      const char* header, int max_n_iteration, float r2_max)
{
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), header);

        int start_x_position  = 0;
        int start_y_position  = 0;

        pixel_t *pixels = (pixel_t*) calloc(window_width * window_height, sizeof(pixel_t));
        if (!pixels)
                return NULL_CALLOC;

        int turn_drawing_on  = ~0;
        int performance_mode = ~0;

        sf::Text fps_counter;
        sf::Clock clock;
        sf::Time current_time = clock.getElapsedTime();
        sf::Time previous_time = current_time;

        while (window.isOpen()) {
                if (performance_mode)
                        calc_mandelbrot_pixels(pixels,  window_width, max_x_coordinate,
                                                        window_height, max_y_coordinate,
                                                        &start_x_position, &start_y_position,
                                                        max_n_iteration, r2_max);
                else
                        slow_calc_mandelbrot_pixels(pixels,  window_width, max_x_coordinate,
                                                             window_height, max_y_coordinate,
                                                             &start_x_position, &start_y_position,
                                                             max_n_iteration, r2_max);
                if (turn_drawing_on) {
                        print_screen(&window, pixels, window_height, window_width);
                        window.display();
                }

                sf::Event event;
                while (window.pollEvent(event))
                        if (event.type == sf::Event::Closed) {
                                window.close();
                        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                                turn_drawing_on = ~turn_drawing_on;
                                printf("Changed drawing mode(%d).\n", turn_drawing_on);
                                break;
                        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                                performance_mode = ~performance_mode;
                                printf("Changed performance mode(%d).\n", performance_mode);
                                break;
                        }

                change_scale(get_pressed_key(), &start_x_position, &start_y_position, &max_x_coordinate, &max_y_coordinate);

                current_time = clock.getElapsedTime();
                double fps = 1.0f / (current_time.asSeconds() - previous_time.asSeconds());

                printf("fps = %lf\n", fps);
                previous_time = current_time;
        }
        free(pixels);

        return 0;
}

int calc_mandelbrot_pixels (pixel_t *pixels, int window_width,        float max_x_coordinate,
                                             int window_height,       float max_y_coordinate,
                                             int *start_x_position,   int *start_y_position,
                                             int max_n_iteration,     float r2_max)
{
        float x_scale_coeff = 2 * max_x_coordinate / (float) window_width;
        float y_scale_coeff = 2 * max_y_coordinate / (float) window_height;

        __m256 avx_x = _mm256_set1_ps(-max_x_coordinate);
        __m256 avx_y = _mm256_set1_ps(-max_y_coordinate);

        __m256 avx_x0 = {};
        __m256 avx_y0 = {};

        __m256 avx_max_x_coord = avx_x;
        __m256 avx_r2_max = _mm256_set1_ps(r2_max);

        __m256 avx_two      = _mm256_set_ps(2, 2, 2, 2, 2, 2, 2, 2);
        __m256 avx_76543210 = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);

        __m256 avx_dx_coeffs = _mm256_mul_ps(avx_76543210, _mm256_set1_ps(x_scale_coeff));

        for (int pixel_y = 0; pixel_y < window_height; pixel_y++) {
                // y = pixel_y * y_scale_coeff + -max_y_coordinate
                avx_y = _mm256_set1_ps((float) (pixel_y + *start_y_position) * y_scale_coeff - max_y_coordinate);
                avx_y0 = avx_y;

                for (int pixel_x = 0; pixel_x < window_width; pixel_x += 8) {
                        avx_y = avx_y0;
                        __m256 avx_scaled_pixel_x = _mm256_set1_ps((float) (pixel_x + *start_x_position) * x_scale_coeff);

                        // x = pixel_x * x_scale_coeff + -max_x_coordinate
                        avx_x = _mm256_add_ps(_mm256_add_ps(avx_scaled_pixel_x, avx_dx_coeffs), avx_max_x_coord);
                        avx_x0 = avx_x;
                        __m256i avx_n_iterations = _mm256_setzero_si256();

                        for (int i = 0; i < max_n_iteration; i++) {
                                __m256 avx_x2 = _mm256_mul_ps(avx_x, avx_x);
                                __m256 avx_y2 = _mm256_mul_ps(avx_y, avx_y);
                                __m256 avx_xy = _mm256_mul_ps(avx_x, avx_y);

                                __m256 avx_r2 = _mm256_add_ps(avx_x2, avx_y2);

                                __m256 avx_cmp = _mm256_cmp_ps(avx_r2, avx_r2_max, _CMP_LT_OQ);
                                if (!_mm256_movemask_ps(avx_cmp))
                                        break;

                                avx_n_iterations = _mm256_sub_epi32(avx_n_iterations, _mm256_castps_si256(avx_cmp));

                                avx_x = _mm256_add_ps(_mm256_sub_ps(avx_x2, avx_y2), avx_x0);
                                avx_y = _mm256_add_ps(_mm256_mul_ps(avx_two, avx_xy), avx_y0);
                        }

                        int *n_iter = (int*) &avx_n_iterations;
                        for (int i = 0; i < 8; i++) {
                                save_pixel(window_width, *(n_iter + i), max_n_iteration, pixels, pixel_x + i, pixel_y);
                        }
                }
        }
        return 0;
}

int slow_calc_mandelbrot_pixels (pixel_t *pixels, int window_width,        float max_x_coordinate,
                                                  int window_height,       float max_y_coordinate,
                                                  int *start_x_position,   int *start_y_position,
                                                  int max_n_iteration,     float r2_max)
{
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
        window_width  += (int) *start_x_position;

        for (int pixel_y = 0; pixel_y < window_height; pixel_y++) {
                for (int pixel_x = 0; pixel_x < window_width; pixel_x++) {
                        y  = -max_y_coordinate + (float)(pixel_y + *start_y_position) * y_scale_coeff;
                        y0 = y;
                        x  = -max_x_coordinate + (float)(pixel_x + *start_x_position) * x_scale_coeff;
                        x0 = x;

                        int i = 0;
                        for (; i <= max_n_iteration; i++) {
                                x2 = x * x;
                                y2 = y * y;
                                xy = x * y;

                                if (x2 + y2 >= r2_max)
                                        break;

                                x = x2 - y2 + x0;
                                y = xy * 2  + y0;
                        }
                        save_pixel(window_width, i, max_n_iteration, pixels, pixel_x, pixel_y);
                }
        }
        return 0;
}
