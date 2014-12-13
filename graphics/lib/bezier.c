/*
Chris Burnham and Victoria Tisdale
11/5/14
function definitions for bezier surfaces and curves
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"

/* bezier curve and surface functions */

// sets the zbuffer flag to 1 and the curve points to the x-axis between 0 and 1
void bezierCurve_init(BezierCurve *b){
	int i;
	b->zBuffer = 1;
	for(i=0; i<4; i++){
		point_set2D( &(b->pts[i]), i / 4.0, 0.0 );
	}
}

// sets the zbuffer flag to 1 and the surface to the x-z plane between (0,0) and (1,1)
void bezierSurface_init(BezierSurface *b){
	int i, j;
	b->zBuffer = 1;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			point_set3D( &(b->pts[i][j]), j / 4.0, 0.0, i / 4.0 );
		}
	}
}

// sets the control points of the bezier curve to the four points in the vlist array
void bezierCurve_set(BezierCurve *b, Point *vlist){
	int i;
	for(i=0; i<4; i++){
		point_copy( &(b->pts[i]), &(vlist[i]) );
	}
}

// sets the control points of the bezier surface to the 16 points in the vlist array
void bezierSurface_set(BezierSurface *b, Point *vlist){
	int i, j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			point_copy( &(b->pts[i][j]), &(vlist[i*4 + j]) );
		}
	}	
}

// sets the zbuffer flag to the given value
void bezierCurve_zBuffer(BezierCurve *b, int flag){
	b->zBuffer = flag;
}

// sets the zbuffer flag to the given value
void bezierSurface_zBuffer(BezierCurve *b, int flag){
	b->zBuffer = flag;
}

// draws the bezier curve, given in screen coordinates, into the image using the given color. The function should be adaptive so that it uses an appropriate number of line segments to draw the curve. For example, if the bounding box of the control points is less than 10 pixels in the largest dimension, then it is reasonable to draw the lines between the control points as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c){
	int i;
	double dimBox, minX, minY;
	BezierCurve *b1, *b2;
	Line *l;
	Point vlist[4];
	Point tmp1, tmp2;
	const int thresh = 10;

	minX = b->pts[0].val[0];
	minY = b->pts[0].val[1];
	for(i=1; i<4; i++){
		if( minX > b->pts[i].val[0] ){
			minX = b->pts[i].val[0];
		}
		if( minY > b->pts[i].val[1] ){
			minY = b->pts[i].val[1];
		}
	}

	dimBox = 0;
	for(i=0; i<4; i++){
		if( (b->pts[i].val[0] - minX) > dimBox ){
			dimBox = b->pts[i].val[0] - minX;
		}
		if( (b->pts[i].val[1] - minY) > dimBox ){
			dimBox = b->pts[i].val[1] - minY;
		}
	}

	if(dimBox > thresh){
		b1 = malloc(sizeof(BezierCurve));
		b2 = malloc(sizeof(BezierCurve));
		bezierCurve_init(b1);
		bezierCurve_init(b2);

		vlist[0] = b->pts[0];
		point_set2D( &(vlist[1]), (b->pts[0].val[0]+b->pts[1].val[0]) / 2.0, (b->pts[0].val[1]+b->pts[1].val[1]) / 2.0 );
		point_set2D( &tmp1, (b->pts[1].val[0]+b->pts[2].val[0]) / 2.0, (b->pts[1].val[1]+b->pts[2].val[1]) / 2.0 );
		point_set2D( &(vlist[2]), (vlist[1].val[0]+tmp1.val[0]) / 2.0, (vlist[1].val[1]+tmp1.val[1]) / 2.0 );
		point_set2D( &tmp2, (b->pts[2].val[0]+b->pts[3].val[0]) / 2.0, (b->pts[2].val[1]+b->pts[3].val[1]) / 2.0 );
		point_set2D( &tmp1, (tmp1.val[0]+tmp2.val[0]) / 2.0, (tmp1.val[1]+tmp2.val[1]) / 2.0 );
		point_set2D( &(vlist[3]), (vlist[2].val[0]+tmp1.val[0]) / 2.0, (vlist[2].val[1]+tmp1.val[1]) / 2.0 );
		bezierCurve_set(b1, vlist);
		bezierCurve_draw(b1, src, c);
		
		vlist[0] = vlist[3];
		vlist[1] = tmp1;
		vlist[2] = tmp2;
		vlist[3] = b->pts[3];
		bezierCurve_set(b2, vlist);
		bezierCurve_draw(b2, src, c);

		free(b1);
		free(b2);
	}
	else{
		l = malloc(sizeof(Line));
		for(i=0; i<3; i++){
			line_set(l, b->pts[i], b->pts[i+1]);
			line_draw(l, src, c);
		}
		free(l);
	}
}

/* bezier curve and surface module functions */

// use the de casteljau algorithm to subdivide the bezier curve divisions times, then add the lines connecting the control points to the module. For example, if divisions is 1, the four origional bezier curve control points will be used to generate eight control points and two new bezier curves. then the algorithm will add six lines to the module, three for each of the smaller bezier curves. 
void module_bezierCurve(Module *m, BezierCurve *b, int divisions){
	int i;
	BezierCurve *b1, *b2;
	Line *l;
	Point vlist[4];
	Point tmp1, tmp2;

	if(divisions != 0){
		b1 = malloc(sizeof(BezierCurve));
		b2 = malloc(sizeof(BezierCurve));
		bezierCurve_init(b1);
		bezierCurve_init(b2);

		vlist[0] = b->pts[0];
		point_set3D( &(vlist[1]), (b->pts[0].val[0]+b->pts[1].val[0]) / 2.0, (b->pts[0].val[1]+b->pts[1].val[1]) / 2.0, (b->pts[0].val[2]+b->pts[1].val[2]) / 2.0 );
		point_set3D( &tmp1, (b->pts[1].val[0]+b->pts[2].val[0]) / 2.0, (b->pts[1].val[1]+b->pts[2].val[1]) / 2.0, (b->pts[1].val[2]+b->pts[2].val[2]) / 2.0 );
		point_set3D( &(vlist[2]), (vlist[1].val[0]+tmp1.val[0]) / 2.0, (vlist[1].val[1]+tmp1.val[1]) / 2.0, (vlist[1].val[2]+tmp1.val[2]) / 2.0 );
		point_set3D( &tmp2, (b->pts[2].val[0]+b->pts[3].val[0]) / 2.0, (b->pts[2].val[1]+b->pts[3].val[1]) / 2.0, (b->pts[2].val[2]+b->pts[3].val[2]) / 2.0 );
		point_set3D( &tmp1, (tmp1.val[0]+tmp2.val[0]) / 2.0, (tmp1.val[1]+tmp2.val[1]) / 2.0, (tmp1.val[2]+tmp2.val[2]) / 2.0 );
		point_set3D( &(vlist[3]), (vlist[2].val[0]+tmp1.val[0]) / 2.0, (vlist[2].val[1]+tmp1.val[1]) / 2.0, (vlist[2].val[2]+tmp1.val[2]) / 2.0 );
		bezierCurve_set(b1, vlist);
		module_bezierCurve(m, b1, divisions - 1);
		
		vlist[0] = vlist[3];
		vlist[1] = tmp1;
		vlist[2] = tmp2;
		vlist[3] = b->pts[3];
		bezierCurve_set(b2, vlist);
		module_bezierCurve(m, b2, divisions - 1);

		free(b1);
		free(b2);
	}
	else{
		l = malloc(sizeof(Line));
		for(i=0; i<3; i++){
			line_set(l, b->pts[i], b->pts[i+1]);
			module_line(m, l);
		}
		free(l);
	}
}

// use the de castelajau algorithm to subdivide the bezier surface divisions times, then draw either the lines connecting the control points, if solid is 0, or draw triangles using the four corner control points. For example, if divisions is 1, the 16 origional bezier curve control points will be used to generate 64 control points and four new bezier surfaces, 1 level of subdivision, and then the algorithm will draw lines or triangles to connect the adjacent control points.
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid){
	int i, j, k;
	Line *l;
	Polygon *p;
	Point vlist[3];
    Vector cross[2];
    Vector nm[3];
	Point grid[7][7];
	Point tmp;
	Point input[16];
	BezierSurface newB;

	if( divisions == 0 ){
		if( solid == 0 ){ // makes a 9 squares
			l = malloc(sizeof(Line));
			for(i=0; i<3; i++){
				for(j=0; j<3; j++){
					line_set(l, b->pts[i][j], b->pts[i][j+1]);
					module_line(m, l);

					line_set(l, b->pts[i][j], b->pts[i+1][j]);
					module_line(m, l);
				}
				line_set(l, b->pts[i][3], b->pts[i+1][3]);
				module_line(m, l);

				line_set(l, b->pts[3][i], b->pts[3][i+1]);
				module_line(m, l);
			}
			free(l);
		}
		else{
			p = polygon_create();
            
            vlist[0] = b->pts[0][0];
            vlist[1] = b->pts[3][0];
            vlist[2] = b->pts[0][3];
            polygon_set(p, 3, vlist);
            
            vector_set(&cross[0], b->pts[0][0].val[0] - b->pts[0][1].val[0], b->pts[0][0].val[1] - b->pts[0][1].val[1], b->pts[0][0].val[2] - b->pts[0][1].val[2]);
            vector_set(&cross[1], b->pts[0][0].val[0] - b->pts[1][0].val[0], b->pts[0][0].val[1] - b->pts[1][0].val[1], b->pts[0][0].val[2] - b->pts[1][0].val[2]);
            vector_cross(&cross[0], &cross[1], &nm[0]);
            
            vector_set(&cross[0], b->pts[3][0].val[0] - b->pts[3][1].val[0], b->pts[3][0].val[1] - b->pts[3][1].val[1], b->pts[3][0].val[2] - b->pts[3][1].val[2]);
            vector_set(&cross[1], b->pts[3][0].val[0] - b->pts[2][0].val[0], b->pts[3][0].val[1] - b->pts[2][0].val[1], b->pts[3][0].val[2] - b->pts[2][0].val[2]);
            vector_cross(&cross[0], &cross[1], &nm[1]);
            
            vector_set(&cross[0], b->pts[0][3].val[0] - b->pts[0][2].val[0], b->pts[0][3].val[1] - b->pts[0][2].val[1], b->pts[0][3].val[2] - b->pts[0][2].val[2]);
            vector_set(&cross[1], b->pts[0][3].val[0] - b->pts[1][3].val[0], b->pts[0][3].val[1] - b->pts[1][3].val[1], b->pts[0][3].val[2] - b->pts[1][3].val[2]);
            vector_cross(&cross[0], &cross[1], &nm[2]);
            printf("\n\n");
            for(i=0; i<4; i++){
				for(j=0; j<4; j++){
                    printf("pts[%i][%i]: ", i, j);
                    point_print(&b->pts[i][j], stdout);
                }
            }
            printf("\nfirst set:\n");
            vector_print(&nm[0], stdout);
            vector_print(&nm[1], stdout);
            vector_print(&nm[2], stdout);
            
            polygon_setNormals(p, 3, nm);
            polygon_setSided(p, 0);
            module_polygon(m, p);
            
            vlist[0] = b->pts[0][3];
            vlist[2] = b->pts[3][3];
            polygon_set(p, 3, vlist);
            
            vector_copy(&nm[0], &nm[2]);
            
            vector_set(&cross[0], b->pts[3][3].val[0] - b->pts[3][2].val[0], b->pts[3][3].val[1] - b->pts[3][2].val[1], b->pts[3][3].val[2] - b->pts[3][2].val[2]);
            vector_set(&cross[1], b->pts[2][3].val[0] - b->pts[3][3].val[0], b->pts[2][3].val[1] - b->pts[3][3].val[1], b->pts[2][3].val[2] - b->pts[3][3].val[2]);
            vector_cross(&cross[0], &cross[1], &nm[2]);
             printf("\nSecond set:\n");
            vector_print(&nm[0], stdout);
            vector_print(&nm[1], stdout);
            vector_print(&nm[2], stdout);
            
            polygon_setNormals(p, 3, nm);
            polygon_setSided(p, 0);
            module_polygon(m, p);
            
			polygon_free(p);
		}
	}
	else{ // makes 2 triangles
		for(i=0; i<4; i++){
			grid[2*i][0] = b->pts[i][0];
			point_set3D( &(grid[2*i][1]), (b->pts[i][0].val[0]+b->pts[i][1].val[0]) / 2.0, (b->pts[i][0].val[1]+b->pts[i][1].val[1]) / 2.0, (b->pts[i][0].val[2]+b->pts[i][1].val[2]) / 2.0 );
			point_set3D( &tmp, (b->pts[i][1].val[0]+b->pts[i][2].val[0]) / 2.0, (b->pts[i][1].val[1]+b->pts[i][2].val[1]) / 2.0, (b->pts[i][1].val[2]+b->pts[i][2].val[2]) / 2.0 );
			point_set3D( &(grid[2*i][2]), (grid[2*i][1].val[0]+tmp.val[0]) / 2.0, (grid[2*i][1].val[1]+tmp.val[1]) / 2.0, (grid[2*i][1].val[2]+tmp.val[2]) / 2.0 );
			point_set3D( &(grid[2*i][5]), (b->pts[i][2].val[0]+b->pts[i][3].val[0]) / 2.0, (b->pts[i][2].val[1]+b->pts[i][3].val[1]) / 2.0, (b->pts[i][2].val[2]+b->pts[i][3].val[2]) / 2.0 );
			point_set3D( &(grid[2*i][4]), (grid[2*i][5].val[0]+tmp.val[0]) / 2.0, (grid[2*i][5].val[1]+tmp.val[1]) / 2.0, (grid[2*i][5].val[2]+tmp.val[2]) / 2.0 );
			point_set3D( &(grid[2*i][3]), (grid[2*i][2].val[0]+grid[2*i][4].val[0]) / 2.0, (grid[2*i][2].val[1]+grid[2*i][4].val[1]) / 2.0, (grid[2*i][2].val[2]+grid[2*i][4].val[2]) / 2.0 );
			grid[2*i][6] = b->pts[i][3];
		}
		for(i=0; i<7; i++){
			point_set3D( &(grid[1][i]), (grid[0][i].val[0]+grid[2][i].val[0]) / 2.0, (grid[0][i].val[1]+grid[2][i].val[1]) / 2.0, (grid[0][i].val[2]+grid[2][i].val[2]) / 2.0 );
			point_set3D( &tmp, (grid[4][i].val[0]+grid[2][i].val[0]) / 2.0, (grid[4][i].val[1]+grid[2][i].val[1]) / 2.0, (grid[4][i].val[2]+grid[2][i].val[2]) / 2.0 );
			point_set3D( &(grid[5][i]), (grid[4][i].val[0]+grid[6][i].val[0]) / 2.0, (grid[4][i].val[1]+grid[6][i].val[1]) / 2.0, (grid[4][i].val[2]+grid[6][i].val[2]) / 2.0 );
			point_set3D( &(grid[2][i]), (grid[1][i].val[0]+tmp.val[0]) / 2.0, (grid[1][i].val[1]+tmp.val[1]) / 2.0, (grid[1][i].val[2]+tmp.val[2]) / 2.0 );
			point_set3D( &(grid[4][i]), (grid[5][i].val[0]+tmp.val[0]) / 2.0, (grid[5][i].val[1]+tmp.val[1]) / 2.0, (grid[5][i].val[2]+tmp.val[2]) / 2.0 );
			point_set3D( &(grid[3][i]), (grid[2][i].val[0]+grid[4][i].val[0]) / 2.0, (grid[2][i].val[1]+grid[4][i].val[1]) / 2.0, (grid[2][i].val[2]+grid[4][i].val[2]) / 2.0 );
		}
		bezierSurface_init(&newB);
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				for(k=0; k<4; k++){
					input[j*4 + k] = grid[3*(i%2) + j][3*(i/2) + k];
				}
			}
			bezierSurface_set(&newB, input);
			module_bezierSurface(m, &newB, divisions - 1, solid);
		}
	}
}

// adds a teapot to the module divisions is how many divisions the bezier surfaces should have, solid tells if it should use lines or be polygons
void module_teapot(Module *m, int divisions, int solid){
	Point vertices[127];
	Point vlist[16];
	Module *rim, *body, *lid, *handle, *spout, *bottom;
	BezierSurface b;
	int i;

	i = -1;
	point_set3D( &(vertices[i=i+1]), 0.2000,  0.0000, 2.70000 );
	point_set3D( &(vertices[i=i+1]), 0.2000, -0.1120, 2.70000 );
  point_set3D( &(vertices[i=i+1]), 0.1120, -0.2000, 2.70000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -0.2000, 2.70000 );
  point_set3D( &(vertices[i=i+1]), 1.3375,  0.0000, 2.53125 );
	point_set3D( &(vertices[i=i+1]), 1.3375, -0.7490, 2.53125 );
  point_set3D( &(vertices[i=i+1]), 0.7490, -1.3375, 2.53125 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.3375, 2.53125 );
  point_set3D( &(vertices[i=i+1]), 1.4375,  0.0000, 2.53125 );
	point_set3D( &(vertices[i=i+1]), 1.4375, -0.8050, 2.53125 );
  point_set3D( &(vertices[i=i+1]), 0.8050, -1.4375, 2.53125 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.4375, 2.53125 );
  point_set3D( &(vertices[i=i+1]), 1.5000,  0.0000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 1.5000, -0.8400, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 0.8400, -1.5000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.5000, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 1.7500,  0.0000, 1.87500 );
	point_set3D( &(vertices[i=i+1]), 1.7500, -0.9800, 1.87500 );
  point_set3D( &(vertices[i=i+1]), 0.9800, -1.7500, 1.87500 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.7500, 1.87500 );
  point_set3D( &(vertices[i=i+1]), 2.0000,  0.0000, 1.35000 );
	point_set3D( &(vertices[i=i+1]), 2.0000, -1.1200, 1.35000 );
  point_set3D( &(vertices[i=i+1]), 1.1200, -2.0000, 1.35000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -2.0000, 1.35000 );
  point_set3D( &(vertices[i=i+1]), 2.0000,  0.0000, 0.90000 );
	point_set3D( &(vertices[i=i+1]), 2.0000, -1.1200, 0.90000 );
  point_set3D( &(vertices[i=i+1]), 1.1200, -2.0000, 0.90000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -2.0000, 0.90000 );
  point_set3D( &(vertices[i=i+1]),-2.0000,  0.0000, 0.90000 );
	point_set3D( &(vertices[i=i+1]), 2.0000,  0.0000, 0.45000 );
  point_set3D( &(vertices[i=i+1]), 2.0000, -1.1200, 0.45000 );
	point_set3D( &(vertices[i=i+1]), 1.1200, -2.0000, 0.45000 );
  point_set3D( &(vertices[i=i+1]), 0.0000, -2.0000, 0.45000 );
	point_set3D( &(vertices[i=i+1]), 1.5000,  0.0000, 0.22500 );
  point_set3D( &(vertices[i=i+1]), 1.5000, -0.8400, 0.22500 );
	point_set3D( &(vertices[i=i+1]), 0.8400, -1.5000, 0.22500 );
  point_set3D( &(vertices[i=i+1]), 0.0000, -1.5000, 0.22500 );
	point_set3D( &(vertices[i=i+1]), 1.5000,  0.0000, 0.15000 );
  point_set3D( &(vertices[i=i+1]), 1.5000, -0.8400, 0.15000 );
	point_set3D( &(vertices[i=i+1]), 0.8400, -1.5000, 0.15000 );
  point_set3D( &(vertices[i=i+1]), 0.0000, -1.5000, 0.15000 );
	point_set3D( &(vertices[i=i+1]),-1.6000,  0.0000, 2.02500 );
  point_set3D( &(vertices[i=i+1]),-1.6000, -0.3000, 2.02500 );
	point_set3D( &(vertices[i=i+1]),-1.5000, -0.3000, 2.25000 );
  point_set3D( &(vertices[i=i+1]),-1.5000,  0.0000, 2.25000 );
	point_set3D( &(vertices[i=i+1]),-2.3000,  0.0000, 2.02500 );
  point_set3D( &(vertices[i=i+1]),-2.3000, -0.3000, 2.02500 );
	point_set3D( &(vertices[i=i+1]),-2.5000, -0.3000, 2.25000 );
  point_set3D( &(vertices[i=i+1]),-2.5000,  0.0000, 2.25000 );
	point_set3D( &(vertices[i=i+1]),-2.7000,  0.0000, 2.02500 );
  point_set3D( &(vertices[i=i+1]),-2.7000, -0.3000, 2.02500 );
	point_set3D( &(vertices[i=i+1]),-3.0000, -0.3000, 2.25000 );
  point_set3D( &(vertices[i=i+1]),-3.0000,  0.0000, 2.25000 );
	point_set3D( &(vertices[i=i+1]),-2.7000,  0.0000, 1.80000 );
  point_set3D( &(vertices[i=i+1]),-2.7000, -0.3000, 1.80000 );
	point_set3D( &(vertices[i=i+1]),-3.0000, -0.3000, 1.80000 );
  point_set3D( &(vertices[i=i+1]),-3.0000,  0.0000, 1.80000 );
	point_set3D( &(vertices[i=i+1]),-2.7000,  0.0000, 1.57500 );
  point_set3D( &(vertices[i=i+1]),-2.7000, -0.3000, 1.57500 );
	point_set3D( &(vertices[i=i+1]),-3.0000, -0.3000, 1.35000 );
  point_set3D( &(vertices[i=i+1]),-3.0000,  0.0000, 1.35000 );
	point_set3D( &(vertices[i=i+1]),-2.5000,  0.0000, 1.12500 );
  point_set3D( &(vertices[i=i+1]),-2.5000, -0.3000, 1.12500 );
	point_set3D( &(vertices[i=i+1]),-2.6500, -0.3000, 0.93750 );
  point_set3D( &(vertices[i=i+1]),-2.6500,  0.0000, 0.93750 );
	point_set3D( &(vertices[i=i+1]),-2.0000, -0.3000, 0.90000 );
  point_set3D( &(vertices[i=i+1]),-1.9000, -0.3000, 0.60000 );
	point_set3D( &(vertices[i=i+1]),-1.9000,  0.0000, 0.60000 );
  point_set3D( &(vertices[i=i+1]), 1.7000,  0.0000, 1.42500 );
	point_set3D( &(vertices[i=i+1]), 1.7000, -0.6600, 1.42500 );
  point_set3D( &(vertices[i=i+1]), 1.7000, -0.6600, 0.60000 );
	point_set3D( &(vertices[i=i+1]), 1.7000,  0.0000, 0.60000 );
  point_set3D( &(vertices[i=i+1]), 2.6000,  0.0000, 1.42500 );
	point_set3D( &(vertices[i=i+1]), 2.6000, -0.6600, 1.42500 );
  point_set3D( &(vertices[i=i+1]), 3.1000, -0.6600, 0.82500 );
	point_set3D( &(vertices[i=i+1]), 3.1000,  0.0000, 0.82500 );
  point_set3D( &(vertices[i=i+1]), 2.3000,  0.0000, 2.10000 );
	point_set3D( &(vertices[i=i+1]), 2.3000, -0.2500, 2.10000 );
  point_set3D( &(vertices[i=i+1]), 2.4000, -0.2500, 2.02500 );
	point_set3D( &(vertices[i=i+1]), 2.4000,  0.0000, 2.02500 );
  point_set3D( &(vertices[i=i+1]), 2.7000,  0.0000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 2.7000, -0.2500, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 3.3000, -0.2500, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 3.3000,  0.0000, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 2.8000,  0.0000, 2.47500 );
	point_set3D( &(vertices[i=i+1]), 2.8000, -0.2500, 2.47500 );
  point_set3D( &(vertices[i=i+1]), 3.5250, -0.2500, 2.49375 );
	point_set3D( &(vertices[i=i+1]), 3.5250,  0.0000, 2.49375 );
  point_set3D( &(vertices[i=i+1]), 2.9000,  0.0000, 2.47500 );
	point_set3D( &(vertices[i=i+1]), 2.9000, -0.1500, 2.47500 );
  point_set3D( &(vertices[i=i+1]), 3.4500, -0.1500, 2.51250 );
	point_set3D( &(vertices[i=i+1]), 3.4500,  0.0000, 2.51250 );
  point_set3D( &(vertices[i=i+1]), 2.8000,  0.0000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 2.8000, -0.1500, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 3.2000, -0.1500, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 3.2000,  0.0000, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 0.0000,  0.0000, 3.15000 );
	point_set3D( &(vertices[i=i+1]), 0.8000,  0.0000, 3.15000 );
  point_set3D( &(vertices[i=i+1]), 0.8000, -0.4500, 3.15000 );
	point_set3D( &(vertices[i=i+1]), 0.4500, -0.8000, 3.15000 );
  point_set3D( &(vertices[i=i+1]), 0.0000, -0.8000, 3.15000 );
	point_set3D( &(vertices[i=i+1]), 0.0000,  0.0000, 2.85000 );
  point_set3D( &(vertices[i=i+1]), 1.4000,  0.0000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 1.4000, -0.7840, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 0.7840, -1.4000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.4000, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 0.4000,  0.0000, 2.55000 );
	point_set3D( &(vertices[i=i+1]), 0.4000, -0.2240, 2.55000 );
  point_set3D( &(vertices[i=i+1]), 0.2240, -0.4000, 2.55000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -0.4000, 2.55000 );
  point_set3D( &(vertices[i=i+1]), 1.3000,  0.0000, 2.55000 );
	point_set3D( &(vertices[i=i+1]), 1.3000, -0.7280, 2.55000 );
  point_set3D( &(vertices[i=i+1]), 0.7280, -1.3000, 2.55000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.3000, 2.55000 );
  point_set3D( &(vertices[i=i+1]), 1.3000,  0.0000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 1.3000, -0.7280, 2.40000 );
  point_set3D( &(vertices[i=i+1]), 0.7280, -1.3000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.3000, 2.40000 );
	point_set3D( &(vertices[i=i+1]), 0.0000,  0.0000, 0.00000 );
	point_set3D( &(vertices[i=i+1]), 1.4250, -0.7980, 0.00000 );
  point_set3D( &(vertices[i=i+1]), 1.5000,  0.0000, 0.07500 );
	point_set3D( &(vertices[i=i+1]), 1.4250,  0.0000, 0.00000 );
  point_set3D( &(vertices[i=i+1]), 0.7980, -1.4250, 0.00000 );
	point_set3D( &(vertices[i=i+1]), 0.0000, -1.5000, 0.07500 );
  point_set3D( &(vertices[i=i+1]), 0.0000, -1.4250, 0.00000 );
	point_set3D( &(vertices[i=i+1]), 1.5000, -0.8400, 0.07500 );
  point_set3D( &(vertices[i=i+1]), 0.8400, -1.5000, 0.07500 );

	rim = module_create();
	body = module_create();
	lid = module_create();
	handle = module_create();
	spout = module_create();
	bottom = module_create();
	
	bezierSurface_init(&b);

	i = -1;	
	vlist[i=i+1] = vertices[102];
	vlist[i=i+1] = vertices[103];
	vlist[i=i+1] = vertices[104];
	vlist[i=i+1] = vertices[105];
	vlist[i=i+1] = vertices[4];
	vlist[i=i+1] = vertices[5];
	vlist[i=i+1] = vertices[6];
	vlist[i=i+1] = vertices[7];
	vlist[i=i+1] = vertices[8];
	vlist[i=i+1] = vertices[9];
	vlist[i=i+1] = vertices[10];
	vlist[i=i+1] = vertices[11];
	vlist[i=i+1] = vertices[12];
	vlist[i=i+1] = vertices[13];
	vlist[i=i+1] = vertices[14];
	vlist[i=i+1] = vertices[15];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(rim, &b, divisions, solid);

	i = -1;
	vlist[i=i+1] = vertices[12];
	vlist[i=i+1] = vertices[13];
	vlist[i=i+1] = vertices[14];
	vlist[i=i+1] = vertices[15];
	vlist[i=i+1] = vertices[16];
	vlist[i=i+1] = vertices[17];
	vlist[i=i+1] = vertices[18];
	vlist[i=i+1] = vertices[19];
  vlist[i=i+1] = vertices[20];
	vlist[i=i+1] = vertices[21];
	vlist[i=i+1] = vertices[22];
	vlist[i=i+1] = vertices[23];
	vlist[i=i+1] = vertices[24];
	vlist[i=i+1] = vertices[25];
	vlist[i=i+1] = vertices[26];
	vlist[i=i+1] = vertices[27];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(body, &b, divisions, solid);

	i = -1;
  vlist[i=i+1] = vertices[24];
	vlist[i=i+1] = vertices[25];
	vlist[i=i+1] = vertices[26];
	vlist[i=i+1] = vertices[27];
	vlist[i=i+1] = vertices[29];
	vlist[i=i+1] = vertices[30];
	vlist[i=i+1] = vertices[31];
	vlist[i=i+1] = vertices[32];
  vlist[i=i+1] = vertices[33];
	vlist[i=i+1] = vertices[34];
	vlist[i=i+1] = vertices[35];
	vlist[i=i+1] = vertices[36];
	vlist[i=i+1] = vertices[37];
	vlist[i=i+1] = vertices[38];
	vlist[i=i+1] = vertices[39];
	vlist[i=i+1] = vertices[40];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(body, &b, divisions, solid);

	i = -1;	
	vlist[i=i+1] = vertices[96];
	vlist[i=i+1] = vertices[96];
	vlist[i=i+1] = vertices[96];
	vlist[i=i+1] = vertices[96];
	vlist[i=i+1] = vertices[97];
	vlist[i=i+1] = vertices[98];
	vlist[i=i+1] = vertices[99];
	vlist[i=i+1] = vertices[100];
	vlist[i=i+1] = vertices[101];
	vlist[i=i+1] = vertices[101];
	vlist[i=i+1] = vertices[101];
	vlist[i=i+1] = vertices[101];
	vlist[i=i+1] = vertices[0];
	vlist[i=i+1] = vertices[1];
	vlist[i=i+1] = vertices[2];
	vlist[i=i+1] = vertices[3];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(lid, &b, divisions, solid);

	i = -1;
	vlist[i=i+1] = vertices[0];
	vlist[i=i+1] = vertices[1];
	vlist[i=i+1] = vertices[2];
	vlist[i=i+1] = vertices[3];
	vlist[i=i+1] = vertices[106];
	vlist[i=i+1] = vertices[107];
	vlist[i=i+1] = vertices[108];
	vlist[i=i+1] = vertices[109];
	vlist[i=i+1] = vertices[110];
	vlist[i=i+1] = vertices[111];
	vlist[i=i+1] = vertices[112];
	vlist[i=i+1] = vertices[113];
	vlist[i=i+1] = vertices[114];
	vlist[i=i+1] = vertices[115];
	vlist[i=i+1] = vertices[116];
	vlist[i=i+1] = vertices[117];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(lid, &b, divisions, solid);

	i = -1;
  vlist[i=i+1] = vertices[41];
	vlist[i=i+1] = vertices[42];
	vlist[i=i+1] = vertices[43];
	vlist[i=i+1] = vertices[44];
	vlist[i=i+1] = vertices[45];
	vlist[i=i+1] = vertices[46];
	vlist[i=i+1] = vertices[47];
	vlist[i=i+1] = vertices[48];
  vlist[i=i+1] = vertices[49];
	vlist[i=i+1] = vertices[50];
	vlist[i=i+1] = vertices[51];
	vlist[i=i+1] = vertices[52];
	vlist[i=i+1] = vertices[53];
	vlist[i=i+1] = vertices[54];
	vlist[i=i+1] = vertices[55];
	vlist[i=i+1] = vertices[56];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(handle, &b, divisions, solid);

	i = -1;
  vlist[i=i+1] = vertices[53];
	vlist[i=i+1] = vertices[54];
	vlist[i=i+1] = vertices[55];
	vlist[i=i+1] = vertices[56];
	vlist[i=i+1] = vertices[57];
	vlist[i=i+1] = vertices[58];
	vlist[i=i+1] = vertices[59];
	vlist[i=i+1] = vertices[60];
	vlist[i=i+1] = vertices[61];
	vlist[i=i+1] = vertices[62];
	vlist[i=i+1] = vertices[63];
	vlist[i=i+1] = vertices[64];
	vlist[i=i+1] = vertices[28];
	vlist[i=i+1] = vertices[65];
	vlist[i=i+1] = vertices[66];
	vlist[i=i+1] = vertices[67];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(handle, &b, divisions, solid);

	i = -1;
	vlist[i=i+1] = vertices[68];
	vlist[i=i+1] = vertices[69];
	vlist[i=i+1] = vertices[70];
	vlist[i=i+1] = vertices[71];
	vlist[i=i+1] = vertices[72];
	vlist[i=i+1] = vertices[73];
	vlist[i=i+1] = vertices[74];
	vlist[i=i+1] = vertices[75];
  vlist[i=i+1] = vertices[76];
	vlist[i=i+1] = vertices[77];
	vlist[i=i+1] = vertices[78];
	vlist[i=i+1] = vertices[79];
	vlist[i=i+1] = vertices[80];
	vlist[i=i+1] = vertices[81];
	vlist[i=i+1] = vertices[82];
	vlist[i=i+1] = vertices[83];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(spout, &b, divisions, solid);

	i = -1;
	vlist[i=i+1] = vertices[80];
	vlist[i=i+1] = vertices[81];
	vlist[i=i+1] = vertices[82];
	vlist[i=i+1] = vertices[83];
	vlist[i=i+1] = vertices[84];
	vlist[i=i+1] = vertices[85];
	vlist[i=i+1] = vertices[86];
	vlist[i=i+1] = vertices[87];
	vlist[i=i+1] = vertices[88];
	vlist[i=i+1] = vertices[89];
	vlist[i=i+1] = vertices[90];
	vlist[i=i+1] = vertices[91];
	vlist[i=i+1] = vertices[92];
	vlist[i=i+1] = vertices[93];
	vlist[i=i+1] = vertices[94];
	vlist[i=i+1] = vertices[95];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(spout, &b, divisions, solid);

	i = -1;
	vlist[i=i+1] = vertices[118];
	vlist[i=i+1] = vertices[118];
	vlist[i=i+1] = vertices[118];
	vlist[i=i+1] = vertices[118];
	vlist[i=i+1] = vertices[124];
	vlist[i=i+1] = vertices[122];
	vlist[i=i+1] = vertices[119];
	vlist[i=i+1] = vertices[121];
  vlist[i=i+1] = vertices[123];
	vlist[i=i+1] = vertices[126];
	vlist[i=i+1] = vertices[125];
	vlist[i=i+1] = vertices[120];
	vlist[i=i+1] = vertices[40];
	vlist[i=i+1] = vertices[39];
	vlist[i=i+1] = vertices[38];
	vlist[i=i+1] = vertices[37];

	bezierSurface_set(&b, vlist);
	module_bezierSurface(bottom, &b, divisions, solid);

	module_rotateX(m, 0, -1);
	for(i=0; i<4; i++){
		module_module(m, rim);
		module_module(m, body);
		module_module(m, lid);
		module_module(m, bottom);
		module_rotateY(m, 0, 1);
	}

	module_module(m, handle);
	module_module(m, spout);
	
	module_scale(m, -1, 1, 1);
	module_rotateY(m, -1, 0);
	
	module_module(m, handle);
	module_module(m, spout);
}


