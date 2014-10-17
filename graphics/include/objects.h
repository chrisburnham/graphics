/* data stuctures and function headers for objects
Chris Burnham
*/

#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include "image.h"

typedef struct{
	double val[4];	//four element vector of doubles
} Point;

typedef struct{
	int zBuffer;	//whether to use the z-buffer, default to true (1)
	Point a;			//end point
	Point b;			//end point 
} Line;

typedef struct{
	double r;			//radius
	Point c;			//center
} Circle;

typedef struct{
	double ra; 		//major axis radius
	double rb;		//minor axis radius
	Point c;			//center
	//double a;		//angle of major axis relative to the x axis
} Ellipse;

typedef struct{
	int zBuffer;		//whether to use the z-buffer, default to true (1)
	int numVertex;	//number of vertices
	Point *vertex;	//vertex info
} Polyline;

/* Point functions */
//set the first two values of the vector to x and y. Set the third value to 0.0 and the fourth value to 1.0
void point_set2D(Point *p, double x, double y);

//set the point’s values to x and y and z. Set the homogeneous coordinate to 1.0
void point_set3D(Point *p, double x, double y, double z);

//set the four values of the vector to x, y, z, and h, respectively
void point_set(Point *p, double x, double y, double z, double h);

//set the four values of the vector to x, y, z, and h, respectively
void point_set1(Point *p, double x, double y, double z);

//copy the point data structure
void point_copy(Point *to, Point *from);

//draw the point into src using color c
void point_draw(Point *p, Image *src, Color c);

//draw the p into src using FPixel c
void point_drawf(Point *p, Image *src, FPixel c);

//print out the point
void point_print(Point *p, FILE *fp);

/* Line functions */
//initialize a 2D line
void line_set2D(Line *l, double x0, double y0, double x1, double y1);

//initialize a line to ta and tb
void line_set(Line *l, Point ta, Point tb);

//set the z-buffer flag to the given value
void line_zBuffer(Line *l, int flag);

//copy the line data structure
void line_copy(Line *to, Line *from);

//draw the line into src using color c
void line_draw(Line *l, Image *src, Color c);

/* Circle functions */
//initialize to center tc and radius tr
void circle_set(Circle *c, Point tc, double tr);

//draw the circle into src using color p
void circle_draw(Circle *c, Image *src, Color p);

//draw a filled circle into src using color p
void circle_drawFill(Circle *c, Image *src, Color p);

/* Ellipse functions */
//initialize an ellipse to location tc and radii ta and tb
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);

//draw into src using color p
void ellipse_draw(Ellipse *e, Image *src, Color p);

//draw a filled ellipse into src using color p
void ellipse_drawFill(Ellipse *e, Image *src, Color p);

/* Polyline functions */
//returns an allocated Polyline pointer initialized so that numVertex is 0 and vertex is NULL
Polyline *polyline_create(void);

//returns an allocated Polyline pointer with the vertex list initialized to the points in vlist
Polyline *polyline_createp(int numV, Point *vlist);

//frees the internal data and the Polyline pointer
void polyline_free(Polyline *p);

//initializes the pre-existing Polyline to an empty Polyline
void polyline_init(Polyline *p);

//initializes the vertex list to the points in vlist. De-allocates/allocates the vertex list for p, as necessary
void polyline_set(Polyline *p, int numV, Point *vlist);

//frees the internal data for a Polyline, if necessary, and sets numVertex to 0 and vertex to NULL
void polyline_clear(Polyline *p);

//sets the z-buffer flag to the given value
void polyline_zBuffer(Polyline *p, int flag);

//De-allocates/allocates space as necessary in the destination Polyline data structure and copies the vertex data from the source polyline (from) to the destination (to)
void polyline_copy(Polyline *to, Polyline *from);

//prints Polyline data to the stream designated by the FILE pointer
void polyline_print(Polyline *p, FILE *fp);

//draw the polyline using color c
void polyline_draw(Polyline *p, Image *src, Color c);


#endif
