/* function declarations for fractal */

#ifndef FRACTALS_H
#define FRACTALS_H

void mandelbrot( Image *dst, long double x0, long double y0, long double dx, int iter );
void julia(Image *dst, long double x0, long double y0, long double dx, int iter );
void complexSquare(long double *x, long double *i);

#endif
