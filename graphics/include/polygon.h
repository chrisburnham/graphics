// Author: Victoria Tisdale, Chris Burnham
// Created: Oct 1, 14

// returns an allocated Polygon pointer initialized so that numVertex is 0 and 
// vertex is NULL.
Polygon *polygon create();

// returns an allocated Polygon pointer with the vertex list initialized to a 
// copy of the points in vlist.
Polygon *polygon createp(int numV, Point *vlist);

// frees the internal data for a Polygon and the Polygon pointer.
void polygon free(Polygon *p);

// initializes the existing Polygon to an empty Polygon.
void polygon init(Polygon *p);

// initializes the vertex array to the points in vlist.
void polygon set(Polygon *p, int numV, Point *vlist);

// frees the internal data and resets the fields.
void polygon clear(Polygon *p);

// sets the z-buffer flag to the given value.
void polygon zBuffer(Polygon *p, int flag);

// De-allocates/allocates space and copies the vertex and color data from one 
// polygon to the other.
void polygon copy(Polygon *to, Polygon *from);

// prints polygon data to the stream designated by the FILE pointer.
void polygon print(Polygon *p, FILE *fp);

// draw the outline of the polygon using color c.
void polygon draw(Polygon *p, Image *src, Color c);

// draw the filled polygon using color c with the scanline rendering algorithm.
void polygon drawFill(Polygon *p, Image *src, Color c);

// draw the filled polygon using color c with the Barycentric coordinates
// algorithm.
void polygon drawFillB(Polygon *p, Image *src, Color c);
