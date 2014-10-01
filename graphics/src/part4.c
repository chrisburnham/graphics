/* Chris Burnham part 4 creative code
*/
#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#include "image.h"
#include "objects.h"


int main(int argc, char *argv[]) {
	Line *l;
	Circle *c;
	Ellipse *e;
	Color daColor;
	Point pt1, pt2;
	Image *src;

	l = malloc(sizeof(Line));
	e = malloc(sizeof(Ellipse));
	c = malloc(sizeof(Circle));
	src = image_create( 1000, 1000 );
	
	point_set2D(&pt1, 500, 300);

	color_set( &daColor, 0.2, 0.6, 0.2 );
	circle_set(c, pt1, 100);
	circle_drawFill(c, src, daColor);

	point_set2D(&pt1, 600, 300);
	point_set2D(&pt2, 600, 600);
	line_set(l, pt1, pt2);
	line_draw(l, src, daColor);

	point_set2D(&pt1, 400, 300);
	point_set2D(&pt2, 400, 600);
	line_set(l, pt1, pt2);
	line_draw(l, src, daColor);

	point_set2D(&pt1, 500, 600);
	ellipse_set(e, pt1, 100, 80);
	ellipse_draw(e, src, daColor);

	image_write( src, "part4.ppm");

	free(l);
	free(e);
	free(c);

	return(0);
}
