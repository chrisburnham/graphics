/*
Chris Burnham
part 5 3D scene
10/19/14
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	const int rows = 600;
  const int cols = 800;
	const int nFrames = 100; //out and back so use a multiple of 2
  View3D view;
  Matrix vtm;
	Polygon side[4];
	Color color[4];
  Polygon temp;
	Point vtex[4];
	int i, t;
	Image *src;
	char filename[256];

	for(i=0;i<6;i++) {
		  polygon_init( &side[i] );
		}

	point_set3D(&vtex[0], 0.0, 0.0, 0.0);
	point_set3D(&vtex[1], 0.0, 5.0, 0.0);
	point_set3D(&vtex[2], 2.0, 5.0, 0.0);
	point_set3D(&vtex[3], 2.0, 0.0, 0.0);
	polygon_set( &side[0], 4, &(vtex[0]) );

	point_set3D(&vtex[0], 2.0, 0.0, 0.0);
	point_set3D(&vtex[1], 2.0, 5.0, 0.0);
	point_set3D(&vtex[2], 2.0, 5.0, 2.0);
	point_set3D(&vtex[3], 2.0, 0.0, 2.0);
	polygon_set( &side[1], 4, &(vtex[0]) );

	point_set3D(&vtex[0], 0.0, 0.0, 2.0);
	point_set3D(&vtex[1], 0.0, 5.0, 2.0);
	point_set3D(&vtex[2], 2.0, 5.0, 2.0);
	point_set3D(&vtex[3], 2.0, 0.0, 2.0);
	polygon_set( &side[2], 4, &(vtex[0]) );

	point_set3D(&vtex[0], 0.0, 0.0, 0.0);
	point_set3D(&vtex[1], 0.0, 5.0, 0.0);
	point_set3D(&vtex[2], 0.0, 5.0, 2.0);
	point_set3D(&vtex[3], 0.0, 0.0, 2.0);
	polygon_set( &side[3], 4, &(vtex[0]) );
	
	point_set1( &(view.vrp), 1.0, -2.0, 1.0 );
	vector_set( &(view.vpn), 0.0, 1.0, 0.0 );
	vector_set( &(view.vup), 0.0, 0.0, 1.0 );
	view.d = 1;
	view.du = 3;
	view.dv = view.du * (float)rows / cols;
	view.f = 0; // front clip plane
	view.b = 10; // back clip plane
	view.screenx = cols;
	view.screeny = rows;

	src = image_create(rows, cols);

	polygon_init( &temp );

	color_set(&color[0], 0.8, 0.1, 0.1);
	color_set(&color[1], 0.1, 0.8, 0.1);
	color_set(&color[2], 0.1, 0.1, 0.8);
	color_set(&color[3], 0.9, 0.9, 0.1);

	for(t=0; t<nFrames; t++){
		image_reset(src);
		point_set1( &(view.vrp), 1.0 - (t/30.0), -5.0 + (t/10.0), 1.0 ); 
		vector_set( &(view.vpn), 0.0 + (t/20.0), 1.0, 0.0 );

		matrix_setView3D( &vtm, &view );

		for(i=0; i<4; i++){
			polygon_copy( &temp, &side[i] );
			matrix_xformPolygon( &vtm, &temp );

			// normalize by homogeneous coordinate before drawing
			polygon_normalize( &temp );

			polygon_drawFill( &temp, src, color[i] );
		}

		sprintf(filename, "frame-%04d.ppm", t );
    image_write( src, filename );
	}

	system("convert frame-*.ppm flyby.gif");
	system("rm frame-*.ppm");

	polygon_dealloc(&temp);	

	for(i=0; i<4; i++){
		polygon_dealloc(&(side[i]));
	}
	image_free(src);

	return(0);
}
