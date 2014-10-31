/*
Chris Burnham and Victoria Tisdale
Blue Cube test function
10/30/14
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	Image *src;
	View3D view;
	Matrix vtm, gtm;
	DrawState *ds;
	Module *scene;
	Color blue;

	// set up the view
  point_set3D( &(view.vrp), 5, 7, 6 );
  vector_set( &(view.vpn), -1, -1.2, -1.1 );
  vector_set( &(view.vup), 0, 1, 0 );
  view.d = 2.2;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 50;
  view.screenx = 640;
  view.screeny = 360;

  matrix_setView3D( &vtm, &view );
  matrix_identity( &gtm );

	color_set(&blue, .1, .1, .9);

	scene = module_create();
	module_color( scene, &blue );
	module_cube(scene, 0);

	src = image_create( 360, 640 );
  ds = drawstate_create();

	module_draw( scene, &vtm, &gtm, ds, NULL, src );

	image_write( src, "cube.ppm" );

	module_delete( scene );
	free(ds);
	image_free( src );

	return(0);
}

