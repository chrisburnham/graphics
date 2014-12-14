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
	int frame, i, nLights;
	Color green, red, blue, white, grey;
	DrawState ds;
    Element *e;
    Light lgt;
    Lighting light;
	Module *jetman, *dome, *jetmanOn, *scene;
	View3D view;
	Matrix VTM, GTM;
	Point vlist[16];
	BezierSurface b;
	//Point p1, p2;
	//Line l;
	const int sides = 20;
	const int divisions = 3;
	const double gravity = 10;
	const double upForce = 6;
	double speed;
	double pos;
	int rows = 600, cols = 800;
	Image *src = image_create(rows, cols);

	color_set(&green, 0.3, 0.6, 0.1 );
	color_set(&red, 0.7, 0.1, 0.1 );
	color_set(&blue, 0.2, 0.2, 0.7 );
	color_set(&white, 1, 1, 1);
    color_set(&grey, .1, .1, .1);

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
    module_bodyColor(jetman, &blue);
    module_surfaceColor(jetman, &white);
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
	module_identity(jetman);
	module_scale(jetman, .5, 4, .5);
	module_translate(jetman, -1, 4.5, 0);
	module_cylinder(jetman, sides);
	module_translate(jetman, 4, 0, 0);
	module_cylinder(jetman, sides);
	module_color(jetman, &green);
    module_bodyColor(jetman, &green);
    module_surfaceColor(jetman, &white);
	module_identity(jetman);
	module_scale(jetman, .7, 2.5, .7);
	module_translate(jetman, 0, 6, -1);
	module_cylinder(jetman, sides);
	module_translate(jetman, 2, 0, 0);
	module_cylinder(jetman, sides);
	
    light_init(&lgt);
    lgt.type = LightPoint;
    color_set(&lgt.color, 0.02, 0.005, 0.003 );
    point_set3D(&lgt.position, 0.0, 0.0, 0.0);
    
	jetmanOn = module_create();
	module_module(jetmanOn, jetman);
	module_color(jetmanOn, &red);
    module_bodyColor(jetmanOn, &red);
    module_surfaceColor(jetmanOn, &white);
	module_scale(jetmanOn, .7, .5, .7);
	module_translate(jetmanOn, 0, 5.5, -1);
	module_cylinder(jetmanOn, sides);
    
    e = element_init(ObjLight, &lgt);
    module_insert(jetmanOn, e);
    
	module_translate(jetmanOn, 2, 0, 0);
	module_cylinder(jetmanOn, sides);
    
    e = element_init(ObjLight, &lgt);
    module_insert(jetmanOn, e);
	
	scene = module_create();
	module_color(scene, &white); // would probs add background to this
	module_bodyColor(scene, &white);
    module_surfaceColor(scene, &white);
    
    
    i = -1;
	point_set3D( &(vlist[i=i+1]), -10, -9, 10);
	point_set3D( &(vlist[i=i+1]), -9, -4, 11);
	point_set3D( &(vlist[i=i+1]), -11, 3, 10);
	point_set3D( &(vlist[i=i+1]), -10, 10, 11);
    
	point_set3D( &(vlist[i=i+1]), -3, -12, 12);
	point_set3D( &(vlist[i=i+1]), -2, -2, 10);
	point_set3D( &(vlist[i=i+1]), -4, 4, 11);
	point_set3D( &(vlist[i=i+1]), -2, 9, 10);
    
	point_set3D( &(vlist[i=i+1]), 3, -10, 10);
	point_set3D( &(vlist[i=i+1]), 4, -2, 11);
	point_set3D( &(vlist[i=i+1]), 3, 2, 10);
	point_set3D( &(vlist[i=i+1]), 2, 12, 10);
    
	point_set3D( &(vlist[i=i+1]), 12, -9, 10);
	point_set3D( &(vlist[i=i+1]), 9, -3, 11);
	point_set3D( &(vlist[i=i+1]), 10, 3, 12);
	point_set3D( &(vlist[i=i+1]), 10, 11, 11);
    
	bezierSurface_init(&b);
	bezierSurface_set(&b, vlist);

    module_scale(scene, 3, 3, 1);
    module_bezierSurface(scene, &b, divisions+3, 1);
	
	point_set3D(&(view.vrp), 0.0, 6.0, -5.0 );
	vector_set( &(view.vpn), 0.0, 0.0, 1.0 );
	vector_set( &(view.vup), 0.0, 1.0, 0.0 );
	view.d = 1.0;
	view.du = 3.0;
	view.dv = 3.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 50.0;

	matrix_setView3D( &VTM, &view );

	 // ds.shade = ShadeFrame;
	 //ds.shade = ShadeConstant;
	//ds.shade = ShadeFlat;
    ds.shade = ShadeGouraud;
    //ds.shade = ShadeDepth;
    
    lighting_init(&light);
    lighting_add(&light, LightAmbient, &grey, NULL, NULL, 0, 0);
    lighting_add(&light, LightPoint, &grey, NULL, &(view.vrp), 0, 2);

	pos = 0;
	speed = 0;

    nLights = light.nLights;
    
	for(frame=0;frame<189;frame++) { // 150
	// for(frame=41;frame<43;frame++) {
		//printf("frame: %d\n", frame);
		char buffer[256];

		matrix_identity( &GTM );
		
		 module_draw( scene, &VTM, &GTM, &ds, &light, src );

		matrix_translate(&GTM, 0, pos / 200.0, 0);
		matrix_rotateY(&GTM, sin((M_PI*frame)/72.0), cos((M_PI*frame)/72.0));
		matrix_scale(&GTM, .2, .2, .2);

		pos = pos + speed;
        
        if( frame<45 ){
            speed = speed + upForce;
        }
        
		else if( frame>98 ){
            light.nLights = nLights;
            module_lighting(jetmanOn, &VTM, &GTM, &light);
			module_draw(jetmanOn, &VTM, &GTM, &ds, &light, src );
            
            matrix_translate(&GTM, 3, 0, 0);
            if(frame>99 && frame<187){
                module_lighting(jetmanOn, &VTM, &GTM, &light);
            }
			module_draw(jetmanOn, &VTM, &GTM, &ds, &light, src );
            
            matrix_translate(&GTM, -6, 0, 0);
            if(frame>100 && frame<188){
                module_lighting(jetmanOn, &VTM, &GTM, &light);
            }
			module_draw(jetmanOn, &VTM, &GTM, &ds, &light, src );
            
			speed = speed + upForce;
            
            sprintf(buffer, "jetman-frame%03d.ppm", frame);
            image_write(src, buffer);
            image_reset(src);
		}
		else{
            light.nLights = nLights;
            module_lighting(jetman, &VTM, &GTM, &light);
			module_draw(jetman, &VTM, &GTM, &ds, &light, src );
            
            matrix_translate(&GTM, 3, 0, 0);
            module_lighting(jetman, &VTM, &GTM, &light);
			module_draw(jetman, &VTM, &GTM, &ds, &light, src );
            
            matrix_translate(&GTM, -6, 0, 0);
            module_lighting(jetman, &VTM, &GTM, &light);
			module_draw(jetman, &VTM, &GTM, &ds, &light, src );
            
			speed = speed - gravity;
            
            sprintf(buffer, "jetman-frame%03d.ppm", frame);
            image_write(src, buffer);
            image_reset(src);
		}

		
	}

	 system("convert jetman-frame*.ppm jetman.gif");
	 system("rm jetman-frame*.ppm");

	image_free( src );

	module_delete( dome );
	module_delete( jetman );
	module_delete( jetmanOn );
	
	return(0);
}
