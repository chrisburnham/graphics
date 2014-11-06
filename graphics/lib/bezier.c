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
	Point grid[7][7];
	Point tmp;
	Point input[16];
	BezierSurface newB;

	if( divisions == 0 ){
		if( solid == 0 ){
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
			for(i=0; i<3; i++){
				for(j=0; j<3; j++){
					vlist[0] = b->pts[i][j];
					vlist[1] = b->pts[i+1][j];
					vlist[2] = b->pts[i][j+1];
					polygon_set(p, 3, vlist);
					module_polygon(m, p);

					vlist[0] = b->pts[i][j+1];
					vlist[2] = b->pts[i+1][j+1];
					polygon_set(p, 3, vlist);
					module_polygon(m, p);
				}
			}
			polygon_free(p);
		}
	}
	else{
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




