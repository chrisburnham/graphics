/* 
Chris Burnham
11/8/14
Jetman animation
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int frame, i;
	Color green, red, blue;
	DrawState ds;
	Module *jetman, *dome;
	View3D view;
	Matrix VTM, GTM;
	Point vlist[16];
	BezierSurface b;
	const int sides = 20;
	const int divisions = 2;
	int rows = 600, cols = 800;
	Image *src = image_create(rows, cols);

	color_set(&green, 0.2, 0.7, 0.2 );
	color_set(&red, 0.6, 0.3, 0.1 );
	color_set(&blue, 0.2, 0.2, 0.7 );

	i = -1;
	point_set3D( &(vlist[i=i+1]), -2, 0, -2);
	point_set3D( &(vlist[i=i+1]), -3, 0, -1);
	point_set3D( &(vlist[i=i+1]), -3, 0, 1);
	point_set3D( &(vlist[i=i+1]), -2, 0, 2);

	point_set3D( &(vlist[i=i+1]), -1, 0, -3);
	point_set3D( &(vlist[i=i+1]), -1, 3, -1);
	point_set3D( &(vlist[i=i+1]), -1, 3, 1);
	point_set3D( &(vlist[i=i+1]), -1, 0, 3);

	point_set3D( &(vlist[i=i+1]), 1, 0, -3);
	point_set3D( &(vlist[i=i+1]), 1, 3, -1);
	point_set3D( &(vlist[i=i+1]), 1, 3, 1);
	point_set3D( &(vlist[i=i+1]), 1, 0, 3);

	point_set3D( &(vlist[i=i+1]), 2, 0, -2);
	point_set3D( &(vlist[i=i+1]), 3, 0, -1);
	point_set3D( &(vlist[i=i+1]), 3, 0, 1);
	point_set3D( &(vlist[i=i+1]), 2, 0, 2);

	bezierSurface_init(&b);
	bezierSurface_set(&b, vlist);

	dome = module_create();
	module_bezierSurface(dome, &b, divisions, 1);

	jetman = module_create();
	module_color(jetman, &blue);
	module_scale2D(jetman, 1, 5);
	module_cylinder(jetman, sides);
	module_translate(jetman, 2, 0, 0);
	module_cylinder(jetman, sides);
	module_translate(jetman, -1.5, 6, 0);
	module_scale(jetman, 2, .8, 1);
	module_cylinder(jetman, sides);
	module_identity(jetman);
	module_scale(jetman, .5, .7, .5);
	module_translate(jetman, 1, 10, 0);
	module_module(jetman, dome);
	module_identity(jetman);
	module_rotateX(jetman, -1, 0);
	module_scale(jetman, .5, .7, .5);
	module_translate(jetman, 1, 10, 0);
	module_module(jetman, dome);
	module_color(jetman, &green);
	module_identity(jetman);
	module_scale(jetman, .7, 2.5, .7);
	module_translate(jetman, 0, 6, -1);
	module_cylinder(jetman, sides);
	module_translate(jetman, 2, 0, 0);
	module_cylinder(jetman, sides);
	

	
	


	

	point_set3D(&(view.vrp), 0.0, 8.0, -5.0 );
	vector_set( &(view.vpn), 0.0, 0.0, 1.0 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 1.0;
	view.du = 3.0;
	view.dv = 3.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );

	ds.shade = ShadeFrame;

	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( jetman, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "jetman-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	system("convert jetman-frame*.ppm jetman.gif");
	system("rm jetman-frame*.ppm");

	image_free( src );

	module_delete( jetman );
	
	return(0);
}
