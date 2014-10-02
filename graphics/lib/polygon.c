//
//  Library file for polygon functions
//  
//
//  Created by chris burnham on 10/1/14.
//  Authors: Chris Burnham, Victoria Tisdale
//

#include <stdlib.h>
#include <stdio.h>
#include "polygon.h"
#include "objects.h"
#include "ppmIO.h"
#include "image.h"



// Note for Victoria: as of 7:20 on 8/1 nothing in here has been tested.



// returns an allocated Polygon pointer initialized so that numVertex is 0 and
// vertex is NULL.
Polygon *polygon_create(){
    Polygon *p;
    
    p = malloc(sizeof(Polygon));
    
    p->zBuffer = 1;
    p->nVertex = 0;
    p->vertex = NULL;
    
    return p;
}

// returns an allocated Polygon pointer with the vertex list initialized to a
// copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist){
    Polygon *p;
    int i;
    
	p = malloc(sizeof(Polygon));
    
	p->zBuffer = 1;
	p->nVertex = numV;
	p->vertex = malloc(sizeof(Point) * numV);
	for(i=0; i<numV; i++){
        p->vertex[i] = vlist[i];
    }
}

// frees the internal data for a Polygon and the Polygon pointer.
void polygon_free(Polygon *p){
    if(p){
		if(p->vertex){
			free(p->vertex);
		}
		free(p);
	}
}

// initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p){
    if(p){
		p->zBuffer = 1;
		p->numVertex = 0;
        if(p->vertex){
			free(p->vertex);
		}
		p->vertex = NULL;
	}
}

// initializes the vertex array to the points in vlist.
void polygon_set(Polygon *p, int numV, Point *vlist){
    int i;
    
	if(p){
		if(p->vertex){
			free(p->vertex);
		}
		p->zBuffer = 1;
		p->nVertex = numV;
		p->vertex = malloc(sizeof(Point) * numV);
        for(i=0; i<numV; i++){
            p->vertex[i] = vlist[i];
        }
	}
}

// frees the internal data and resets the fields.
void polygon_clear(Polygon *p){
    if(p){
		if(p->vertex){
			free(p->vertex);
		}
		polygon_init(p);
	}
}

// sets the z-buffer flag to the given value.
void polygon_zBuffer(Polygon *p, int flag){
    if(p){
		p->zBuffer = flag;
	}
}

// De-allocates/allocates space and copies the vertex and color data from one
// polygon to the other.
void polygon_copy(Polygon *to, Polygon *from){
    if(from){
		polyline_set(to, from->numVertex, from->vertex);
		polyline_zBuffer(to, from->zBuffer);
	}
}

// prints polygon data to the stream designated by the FILE pointer.
void polygon_print(Polygon *p, FILE *fp){
    int i;
	
	if(p){
		fprintf(fp, "Polyline:\n");
		if(p->vertex){
			for(i = 0; i < p->numVertex; i++){
				fprintf(fp, "(%f,%f,%f) , ", p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2]);
			}
			fprintf(fp, "\n");
		}
	}
}

// draw the outline of the polygon using color c.
void polygon_draw(Polygon *p, Image *src, Color c){
    int i;
	Line edge;
    
	if(p){
		if(p->vertex){
			for(i = 1; i < p->nVertex; i++){
				line_set(edge, p->vertex[i-1],  p->vertex[i]);
				line_draw(edge, src, c);
			}
            line_set(edge, p->vertex[0],  p->vertex[nVertex - 1]);
            line_draw(edge, src, c);
		}
	}
}

// draw the filled polygon using color c with the scanline rendering algorithm.
void polygon_drawFill(Polygon *p, Image *src, Color c);

// draw the filled polygon using color c with the Barycentric coordinates
// algorithm. Will draw for the zero case.
void polygon_drawFillB(Polygon *p, Image *src, Color c){
    float a, b, g, dXab, dYab, dXac, dYac, xyabDiff, xyacDiff, abDenom, acDenom;
    int minX, maxX, minY, maxY;
    int i, j;
    float epsilon;
    
    if(p){
        if(p->nVertex == 3){
            maxX = -1;
            maxY = -1;
            minX = (src->cols) + 10;
            minY = (src->rows) + 10;
            
            for(i=0; i<3; i++){
                if( ((int)(p->vertex[i].val[0]) + 1) > maxX ){
                    maxX = (int)(p->vertex[i].val[0]) + 1;
                }
                if( (int)(p->vertex[i].val[0]) < minX ){
                    minX = (int)(p->vertex[i].val[0]);
                }
                if( ((int)(p->vertex[i].val[1]) + 1) > maxY ){
                    maxY = (int)(p->vertex[i].val[1]) + 1;
                }
                if( (int)(p->vertex[i].val[1]) < minY ){
                    minY = (int)(p->vertex[i].val[1]);
                }
            }
            
            if( maxX > minX && maxY > minY ){
            // if this is false something about our bounding box is wrong

                
                epsilon = -.00001; // closer than this we consider zero
                
                //calculating peices for beta and gama that are not point dependent
                dXab = p->vertex[1].val[0] - p->vertex[0].val[0];
                dYab = p->vertex[0].val[1] - p->vertex[1].val[1];
                dXac = p->vertex[2].val[0] - p->vertex[0].val[0];
                dYac = p->vertex[0].val[1] - p->vertex[2].val[1];
                
                xyabDiff = ((p->vertex[0].val[0]) * (p->vertex[1].val[1]))
                - ((p->vertex[1].val[0]) * (p->vertex[0].val[1]));
                
                xyacDiff = ((p->vertex[0].val[0]) * (p->vertex[2].val[1]))
                - ((p->vertex[2].val[0]) * (p->vertex[0].val[1]));
                
                abDenom = (dYab * (p->vertex[2].val[0]))
                + (dYab * (p->vertex[2].val[1])) + xyabDiff;
                
                acDenom = (dYac * (p->vertex[1].val[0]))
                + (dYac * (p->vertex[1].val[1])) + xyacDiff;
                
                for(i=minX; i<maxX; i++){ // loop through bounding box
                    for(j=minY; j<maxY; j++){
                        b = ( (dYac * (i + 0.5)) + (dXac * (j + 0.5)) + xyacDiff)
                        / acDenom;
                        if( b > epsilon ){
                            g = ( (dYab * (i + 0.5)) + (dXab * (j + 0.5))
                            + xyabDiff) / abDenom;
                            if( g > epsilon ){
                                a = 1.0 - b - g;
                                if( a > epsilon ){
                                    image_setColor(src, j, i, c);
                                }
                            }
                        }
                    }
                }
            }
        }
        else{
            printf("Not given a triangle. Given polygon has %i verticies", p->nVertex);
        }
    }
}




