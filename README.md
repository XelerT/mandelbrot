# Mandelbrot
- There are 3 types of perfarmance improvement.

![alt Mandelbrot](https://github.com/XelerT/mandelbrot/blob/main/img/Screenshot_20230402_010044.png)

# Install & Run
```
> git clone
> make
> make run
```
---

## How to control

### Moving and scaling

![alt Moving](https://github.com/XelerT/mandelbrot/blob/main/img/Screenshot_20230402_011331.png)

> - Use arrows to move along the plane.
> - Use Plus to get closer to mandelbrot.
> - Use Minus to get move away.

---
### Performance modes

You can change ways of mandelbrot calculation using **P** on keyboard. Using **D**, you can turn on/off drawing of mandelbrot.

# Performance
        Tests was done using -mavx2 and -O1 optimization flags on 100 cycles of drawing mandelbrot.
#### No performance modification
![alt Slow]()
        Main part of cpu load was on this part of code.
        ```
       │    │x2 = x * x;
 21.20 │10c:│  vmulss    %xmm2,%xmm2,%xmm1
       │    │y2 = y * y;
  5.99 │    │  vmulss    %xmm0,%xmm0,%xmm3
       │    │xy = x * y;
  2.62 │    │  vmulss    %xmm0,%xmm2,%xmm0
       │    │if (x2 + y2 >= coords->r2_max)
 25.27 │    │  vaddss    %xmm3,%xmm1,%xmm2
 17.98 │    │  vcomiss   %xmm6,%xmm2
  4.55 │    │↑ jae       a5
       │    │x = x2 - y2 + x0;
  0.11 │    │  vsubss    %xmm3,%xmm1,%xmm1
 11.06 │    │  vaddss    %xmm4,%xmm1,%xmm2
       │    │y = xy * 2  + y0;
  0.09 │    │  vaddss    %xmm0,%xmm0,%xmm0
  6.29 │    │  vaddss    %xmm5,%xmm0,%xmm0
        ```

#### AVX Parallelism
        ![alt Fast]()
        Main parts of cpu load was on this part of code.
        ```
        | __m256 avx_x2 = _mm256_mul_ps(avx_x, avx_x);
        | __m256 avx_y2 = _mm256_mul_ps(avx_y, avx_y);
        | __m256 avx_xy = _mm256_mul_ps(avx_x, avx_y);
   0.06 │1b0:   vmulps       %ymm0,%ymm0,%ymm1
   0.01 │       vmulps       %ymm2,%ymm2,%ymm4
   9.45 │       vmulps       %ymm2,%ymm0,%ymm2

        | __m256 avx_r2 = _mm256_add_ps(avx_x2, avx_y2);
  11.36 │       vaddps       %ymm4,%ymm1,%ymm0

        | __m256 avx_cmp = _mm256_cmp_ps(avx_r2, avx_r2_max, _CMP_LT_OQ);
  27.18 │       vcmplt_oqps  %ymm7,%ymm0,%ymm0

        |       if (!_mm256_movemask_ps(avx_cmp))
        |               break;
  17.81 │       vmovmskps    %ymm0,%edx

        │     __m256 avx_x2 = _mm256_mul_ps(avx_x, avx_x);
  13.72 │       test         %edx,%edx
   0.16 │     ↑ je           c4
        ```

## Credits
        - Created by Alex Taranov during 1 year of X courses.

<sup align = "center"> Spring 2023 </sup>

