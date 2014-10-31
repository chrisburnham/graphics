/*
Chris Burnham
Part 3 scene
Halloween 2014
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
	Image *src;
	View2D view;
	Point vrp;
	Vector xaxis;
	Matrix vtm, gtm;
	DrawState *ds;
	Module *torso, *narrow, *wide, *armyN, *armyW, *tripleN, *tripleW, *scene;
	Point p[4];
	Line l;
	int i, j;
	char filename[256];
	const int screenX = 1200;
	const int screenY = 1000;
	const int nFrames = 10; // will have 3n + 5 frames

	// set up the view
	point_set2D( &vrp, 5, 5 );
  vector_set( &xaxis, 1, 0, 0 );
  view2D_set( &view, &vrp, 40, &xaxis, screenX, screenY );
  matrix_setView2D( &vtm, &view );

  matrix_identity( &gtm );

	// torso for both person models
	torso = module_create();

	point_set2D(&(p[0]), 0, 4);
	point_set2D(&(p[1]), 0, 0);
	point_set2D(&(p[2]), -2, 1);
	point_set2D(&(p[3]), 2, 1);

	for(i=1; i<4; i++){
		line_set(&l, p[i], p[0]);
		module_line( torso, &l );
	}

	module_scale2D(torso, 2, 2);
	module_translate2D(torso, -1, 4);
	module_cube(torso, 0); 	

	// person model with narrow stance
	narrow = module_create();
	point_set2D(&(p[0]), 1, 5);
	point_set2D(&(p[2]), 2, 0);

	for(i=1; i<3; i++){
		line_set(&l, p[i], p[0]);
		module_line( narrow, &l );
	}
	
	module_translate2D(narrow, 1, 5);
	module_module(narrow, torso);

	// person model with wide stance
	wide = module_create();
	point_set2D(&(p[0]), 3, 5);
	point_set2D(&(p[2]), 6, 0);

	for(i=1; i<3; i++){
		line_set(&l, p[i], p[0]);
		module_line( wide, &l );
	}
	
	module_translate2D(wide, 3, 5);
	module_module(wide, torso);

	// army of narrow legged people
	armyN	= module_create();
	
	module_scale2D(armyN, 0.1, 0.1);
	
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			module_module(armyN, narrow);
			module_translate2D(armyN, 1, 0);
		}
		module_translate2D(armyN, -8, 1.1);
	}

	// army of wide legged people
	armyW	= module_create();
	
	module_scale2D(armyW, 0.1, 0.1);
	
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			module_module(armyW, wide);
			module_translate2D(armyW, 1, 0);
		}
		module_translate2D(armyW, -8, 1.1);
	}

	// 3 blocks of narrow leggs
	tripleN = module_create();
	
	module_translate2D(tripleN, 0, -8);

	for(i=0; i<3; i++){
		module_module(tripleN, armyN);
		module_translate2D(tripleN, 0, 10);
	}

	// 3 blocks of wide leggs
	tripleW = module_create();
	
	module_translate2D(tripleW, 0, -8);

	for(i=0; i<3; i++){
		module_module(tripleW, armyW);
		module_translate2D(tripleW, 0, 10);
	}

	src = image_create( view.screeny, view.screenx );
  ds = drawstate_create();

	scene = module_create();

	for(i=0; i<nFrames; i++){
		image_reset(src);
		module_clear(scene);

		module_translate2D(scene, .9*i - 12, 0);
		module_module(scene, tripleW);
		module_identity(scene);
		module_translate2D(scene, -.9*i + 13, 0);
		module_module(scene, tripleW);
		
		module_draw( scene, &vtm, &gtm, ds, NULL, src );

		sprintf(filename, "frame-%04d.ppm", 3*i );
    image_write( src, filename );


		image_reset(src);
		module_clear(scene);

		module_translate2D(scene, .3 + .9*i - 12, 0);
		module_module(scene, tripleN);
		module_identity(scene);
		module_translate2D(scene, -.3 -.9*i + 13, 0);
		module_module(scene, tripleN);
		
		module_draw( scene, &vtm, &gtm, ds, NULL, src );

		sprintf(filename, "frame-%04d.ppm", 3*i + 1 );
    image_write( src, filename );


		image_reset(src);
		module_clear(scene);

		module_translate2D(scene, .6 + .9*i - 12, 0);
		module_module(scene, tripleN);
		module_identity(scene);
		module_translate2D(scene, -.6 -.9*i + 13, 0);
		module_module(scene, tripleN);
		
		module_draw( scene, &vtm, &gtm, ds, NULL, src );

		sprintf(filename, "frame-%04d.ppm", 3*i + 2 );
    image_write( src, filename );
		
	}

	for(i=0; i<5; i++){
		image_fillrgb(src, .9 - (i/6.0), .1 - (i/50.0), .1 - (i/50.0));
		sprintf(filename, "frame-%04d.ppm", 3*nFrames + i );
    image_write( src, filename );
	}

	system("convert -delay 10 frame-*.ppm army.gif");
	system("rm frame-*.ppm");

	module_delete(torso);
	module_delete(narrow);
	module_delete(wide);
	module_delete(armyN);
	module_delete(armyW);
	module_delete(tripleN);
	module_delete(tripleW);

	free(ds);

	image_free(src);

	return(0);
}
