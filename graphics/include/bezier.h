/*
Chris Burnham and Victoria Tisdale
11/5/14
Data structures and Function declarations for bezier curves and surfaces
*/

#ifndef BEZIER_H
#define BEZIER_H

typedef struct {
	int zBuffer;
	Point pts[4];
} BezierCurve;

typedef struct {
	int zBuffer;
	Point pts[4][4];
} BezierSurface;

/* bezier curve and surface functions */

// sets the zbuffer flag to 1 and the curve points to the x-axis between 0 and 1
void bezierCurve_init(BezierCurve *b);

// sets the zbuffer flag to 1 and the surface to the x-z plane between (0,0) and (1,1)
void bezierSurface_init(BezierSurface *b);

// sets the control points of the bezier curve to the four points in the vlist array
void bezierCurve_set(BezierCurve *b, Point *vlist);

// sets the control points of the bezier surface to the 16 points in the vlist array
void bezierSurface_set(BezierSurface *b, Point *vlist);

// sets the zbuffer flag to the given value
void bezierCurve_zBuffer(BezierCurve *b, int flag);

// sets the zbuffer flag to the given value
void bezierSurface_zBuffer(BezierCurve *b, int flag);

// draws the bezier curve, given in screen coordinates, into the image using the given color. The function should be adaptive so that it uses an appropriate number of line segments to draw the curve. For example, if the bounding box of the control points is less than 10 pixels in the largest dimension, then it is reasonable to draw the lines between the control points as an approximation to the curve.
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);

/* bezier curve and surface module functions */

// use the de casteljau algorithm to subdivide the bezier curve divisions times, then add the lines connecting the control points to the module. For example, if divisions is 1, the four origional bezier curve control points will be used to generate eight control points and two new bezier curves. then the algorithm will add six lines to the module, three for each of the smaller bezier curves. 
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);

// use the de castelajau algorithm to subdivide the bezier surface divisions times, then draw either the lines connecting the control points, if solid is 0, or draw triangles using the four corner control points. For example, if divisions is 1, the 16 origional bezier curve control points will be used to generate 64 control points and four new bezier surfaces, 1 level of subdivision, and then the algorithm will draw lines or triangles to connect the adjacent control points.
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);

// adds a teapot to the module divisions is how many divisions the bezier surfaces should have, solid tells if it should use lines or be polygons
void module_teapot(Module *m, int divisions, int solid);

#endif
