/* Chris Burnham and Victoria Tisdale
11/7/14
test for the teapot
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	int frame;
	Color purple, grey, white;
	DrawState ds;
    Lighting light;
	Module *teapot;
	View3D view;
	Matrix VTM, GTM;
	const int divisions = 4;
	int rows = 600, cols = 800;
	Image *src = image_create(rows, cols);

	color_set(&purple, 0.6, 0.1, 0.7 );
    color_set(&grey, .1, .1, .1);
    color_set(&white, 1, 1, 1);

	teapot = module_create();
	module_teapot(teapot, divisions, 1);

	// set up the drawstate
	drawstate_setColor(&ds, purple);
    drawstate_setBody(&ds, purple);
    drawstate_setSurface(&ds, white);
    //ds.shade = ShadeDepth;
    ds.shade = ShadeGouraud;
    

	// set up the view
	point_set3D(&(view.vrp), 0.0, 1.0, -5.0 );
	vector_set( &(view.vpn), 0.0, 0.0, 1.0 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 0.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 10.0;
    
    lighting_init(&light);
    lighting_add(&light, LightAmbient, &grey, NULL, NULL, 0, 0);
    lighting_add(&light, LightPoint, &grey, NULL, &(view.vrp), 0, 2);

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );

	// Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( teapot, &VTM, &GTM, &ds, &light, src );

		sprintf(buffer, "teapot-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	system("convert teapot-frame*.ppm teapot.gif");
	system("rm teapot-frame*.ppm");

	// clean up
	image_free( src );

	module_delete( teapot );

	return(0);
}

