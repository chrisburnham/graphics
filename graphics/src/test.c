#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <math.h>
#include "ppmIO.h"

#include "image.h"
#include "objects.h"

#define NUMLINES 200
#define TIMING 5
#define ROWS 200
#define COLS 200

int main(int argc, char *argv[]) {
	int i, j;
	Color white, daColor;
	Image *src;
	Point a, b;
	Line *l;
	//Ellipse *e;

	l = malloc(sizeof(Line));	

	color_set( &white, 1.0, 1.0, 1.0);	

	src = image_create(1000,1000);

	for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
      image_setColor(src, i, j, white );
    }
  }
	
	point_set2D(&a, 500, 500);
	point_set2D(&b, 500, 700);
	color_set(&daColor, 1.0, 0.0, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 600, 700);
	color_set(&daColor, 0.0, 1.0, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 700, 700);
	color_set(&daColor, 1.0, 1.0, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 700, 600);
	color_set(&daColor, 0.0, 0.0, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 700, 500);
	color_set(&daColor, 0.0, 0.0, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 500, 400);
	color_set(&daColor, 0.0, 1.0, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 300, 500);
	color_set(&daColor, 0.0, 1.0, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 300, 400);
	color_set(&daColor, 0.0, 1.0, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 400, 300);
	color_set(&daColor, 0.4, 1.0, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 600, 300);
	color_set(&daColor, 0.4, 0.4, 0.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 550, 100);
	color_set(&daColor, 0.3, 0.4, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

	point_set2D(&b, 550, 900);
	color_set(&daColor, 0.3, 0.4, 1.0);
	line_set(l, a, b);
	line_draw(l, src, daColor);

/*	e = malloc(sizeof(Ellipse));
	ellipse_set(e, a, 100, 200);
	color_set(&daColor, 0.0, 0.0, 0.0);
	ellipse_drawFill(e, src, daColor); 


	free(e); */
	free(l);
	image_write(src, "test.ppm");
	image_free(src);

	return(0);
}
