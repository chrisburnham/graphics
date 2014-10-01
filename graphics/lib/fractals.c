#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "image.h"
#include "fractals.h"

void complexSquare(long double *x, long double *i){
	long double temp;
	temp = ((*x) * (*x)) - ((*i) * (*i));
	*i = 2 * (*x) * (*i);
	*x = temp;
}

void mandelbrot( Image *dst, long double x0, long double y0, long double dx, int iter ){
	int rows, cols, i, j, k;
	long double real, imag, locr, loci;

	rows = dst->rows;
	cols = dst->cols;
	

	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			k = 0;
			locr = x0 + ( (dx * j) / cols );
			loci = ( -1 * y0 ) + ( (dx * i) / cols );
			real = locr;
			imag = loci;
			while( pow( ((real * real) + (imag * imag)), ((long double)1.0 / 2.0) ) < 2 && k < iter ){
				k = k + 1;
				complexSquare(&real, &imag);
				real = real + locr;
				imag = imag + loci;
			}
			if(k == iter){
				dst->data[i][j].rgb[0] = 0.0;
				dst->data[i][j].rgb[1] = 0.0;
				dst->data[i][j].rgb[2] = 0.0;
			}
			else{
				dst->data[i][j].rgb[0] = ( (float)( (k + 70) % 255 ) / 255.0 );
				dst->data[i][j].rgb[1] = ( (float)( ((k / 3) + 140) % 255 ) / 255.0 );
				dst->data[i][j].rgb[2] = ( (float)( ((k / 9) + 210) % 255 ) / 255.0 );
			}
		}
	}
}

void julia(Image *dst, long double x0, long double y0, long double dx, int iter ){
	int rows, cols, i, j, k;
	long double real, imag, locr, loci;

	rows = dst->rows;
	cols = dst->cols;
	
	locr = 0.3;
	loci = 0.5;

	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			k = 0;
			
			real = x0 + ( (dx * j) / cols );
			imag = ( -1 * y0 ) + ( (dx * i) / cols );
			while( pow( ((real * real) + (imag * imag)), ((long double)1.0 / 2.0) ) < 2 && k < iter ){
				k = k + 1;
				complexSquare(&real, &imag);
				real = real + locr;
				imag = imag + loci;
			}
			if(k == iter){
				dst->data[i][j].rgb[0] = 0.0;
				dst->data[i][j].rgb[1] = 0.0;
				dst->data[i][j].rgb[2] = 0.0;
			}
			else{
				dst->data[i][j].rgb[0] = ( (float)( (k + 70) % 255 ) / 255.0 );
				dst->data[i][j].rgb[1] = ( (float)( ((k / 3) + 140) % 255 ) / 255.0 );
				dst->data[i][j].rgb[2] = ( (float)( ((k / 9) + 210) % 255 ) / 255.0 );
			}
		}
	}
}
