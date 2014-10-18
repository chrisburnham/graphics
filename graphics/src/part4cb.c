/*
	Chris Burnham
	2D image part 4 10/18/14
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"


int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 600;
  const int cols = 800;
	const int numFrames = 50;
	Polygon *tri[27];
	Point vtex[3];
	int i, j, k, l, t;
	const double triH = 0.866025;
	View2D view;
	Matrix vtm;
	Color green;
	char filename[256];
  
	src = image_create(rows, cols);
	color_set(&green, 0.1, 0.8, 0.1);

	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			for(k=0; k<3; k++){
				for(l=0; l<3; l++){
					point_set2D(&vtex[l], (l + 2*k + 4*j + 8*i), ( (l%2)*triH + (k%2)*triH*2 + (j%2)*triH*4 + (i%2)*triH*8));
				}
				tri[9*i+3*j+k] = polygon_createp(3, vtex);
			}
		}
	}

	vector_set( &(view.x), 1.0, 0.0, 0.0 );
	view.screenx = cols;
  view.screeny = rows;
	
	for(t=0; t<numFrames; t++){
		printf("loop%i:\n", t);
		image_fillrgb(src, 0.1, 0.0, 0.2);
		point_set2D( &(view.vrp), 2.0, 2.0 );
		view.dx = 40 - (t/2.0);

		matrix_setView2D( &vtm, &view );

		for(i=0; i<27; i++){
			matrix_xformPolygon(&vtm, tri[i]);
			polygon_drawFillB(tri[i], src, green);
		}

		sprintf(filename, "frame-%04d.ppm", t );
    image_write( src, filename );
	}
	
	system("convert frame-*.ppm cube.gif");
	system("rm frame-*.ppm");

  for(i=0; i<27; i++){
		polygon_free(tri[i]);
	}

	image_free(src);

  return(0);
}
