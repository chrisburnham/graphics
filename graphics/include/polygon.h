// Author: Victoria Tisdale, Chris Burnham
// Created: Oct 1, 14

#ifndef POLYGON_H
#define POLYGON_H

#include "objects.h"
#include "image.h"

typedef struct{ 
	int nVertex; //number of vertices
	Point *vertex; // vertex information
	int zBuffer; // whether to use the z-buffer; should default to true (1)
} Polygon;

// returns an allocated Polygon pointer initialized so that numVertex is 0 and 
// vertex is NULL.
Polygon *polygon_create(void);

// returns an allocated Polygon pointer with the vertex list initialized to a 
// copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist);

// frees the internal data for a Polygon and the Polygon pointer.
void polygon_free(Polygon *p);

// frees the internal data for a Polygon. (for when there isn't really a pointer)
void polygon_dealloc(Polygon *p);

// initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p);

// initializes the vertex array to the points in vlist.
void polygon_set(Polygon *p, int numV, Point *vlist);

// frees the internal data and resets the fields.
void polygon_clear(Polygon *p);

// sets the z-buffer flag to the given value.
void polygon_zBuffer(Polygon *p, int flag);

// De-allocates/allocates space and copies the vertex and color data from one 
// polygon to the other.
void polygon_copy(Polygon *to, Polygon *from);

// prints polygon data to the stream designated by the FILE pointer.
void polygon_print(Polygon *p, FILE *fp);

// draw the outline of the polygon using color c.
void polygon_draw(Polygon *p, Image *src, Color c);

// draw the filled polygon using color c with the scanline rendering algorithm.
void polygon_drawFill(Polygon *p, Image *src, Color c);

void polygon_drawFillwithTexture(Polygon *p, Image *src, Color *c);

// draw the filled polygon using color c with the Barycentric coordinates
// algorithm.
void polygon_drawFillB(Polygon *p, Image *src, Color c);

// draw the filled polygon using color c with the Barycentric coordinates
// algorithm. Will draw for the zero case. uses a texture from a 8by8 color array
void polygon_drawFillBwithTexture(Polygon *p, Image *src, Color *c);

void polygon_normalize(Polygon *p);

#endif
