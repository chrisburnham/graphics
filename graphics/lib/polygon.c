//
//  Library file for polygon functions
//  
//
//  Created by chris burnham on 10/1/14.
//  Authors: Chris Burnham, Victoria Tisdale
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ppmIO.h"
#include "image.h"
#include "list.h"
#include "polygon.h"
#include "scanline.h"

// returns an allocated Polygon pointer initialized so that numVertex is 0 and
// vertex is NULL.
Polygon *polygon_create(){
  Polygon *p;
  
  p = malloc(sizeof(Polygon));
  
  p->zBuffer = 1;
  p->oneSided = 0;
  p->nVertex = 0;
  p->vertex = NULL;
  p->normal = NULL;
  p->s = NULL;
  p->t = NULL;
  
  return p;
}

// returns an allocated Polygon pointer with the vertex list initialized to a
// copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist){
  Polygon *p;
  int i;
  
  p = malloc(sizeof(Polygon));
  
  p->zBuffer = 1;
  p->oneSided = 0;
  p->nVertex = numV;
  p->vertex = malloc(sizeof(Point) * numV);
  for(i=0; i<numV; i++){
    p->vertex[i] = vlist[i];
  }
  p->normal = NULL;
  p->s = NULL;
  p->t = NULL;
  
  return p;
}

// frees the internal data for a Polygon and the Polygon pointer.
void polygon_free(Polygon *p){
  if(p){
      if(p->vertex){
        free(p->vertex);
      }
    if(p->normal){
      free(p->normal);
    }
    free(p);
  }
}

// frees the internal data for a Polygon. (for when there isn't really a pointer)
void polygon_dealloc(Polygon *p){
  if(p){
      if(p->vertex){
        free(p->vertex);
      }
    if(p->normal){
      free(p->normal);
    }
  }
}

// initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p){
  if(p){
    p->zBuffer = 1;
    p->nVertex = 0;
    p->oneSided = 0;
    p->vertex = NULL;
    p->normal = NULL;
    p->s = NULL;
    p->t = NULL;
  }
}

// initializes the vertex array to the points in vlist.
void polygon_set(Polygon *p, int numV, Point *vlist){
  int i;
  
  if(p){
    if(p->vertex){
      free(p->vertex);
    }
    if(p->normal){
      free(p->normal);
    }
    p->zBuffer = 1;
    p->oneSided = 0;
    p->nVertex = numV;
    p->vertex = malloc(sizeof(Point) * numV);
    for(i=0; i<numV; i++){
      p->vertex[i] = vlist[i];
    }
    p->normal = NULL;
    p->s = NULL;
    p->t = NULL;
  }
}

// frees the internal data and resets the fields.
void polygon_clear(Polygon *p){
  if(p){
    if(p->vertex){
      free(p->vertex);
    }
    if(p->normal){
      free(p->normal);
    }
    p->zBuffer = 1;
    p->nVertex = 0;
    p->oneSided = 0;
    p->vertex = NULL;
    p->normal = NULL;
    p->s = NULL;
    p->t = NULL;
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
  int i;
  if(from){
    polygon_set(to, from->nVertex, from->vertex);
    polygon_setNormals(to, from->nVertex, from->normal);
    polygon_setSided(to, from->oneSided);
    polygon_zBuffer(to, from->zBuffer);
    if(from->s && from->t){
      for (i=0; i<from->nVertex; i++){
        polygon_setST(to, i, from->s[i], from->t[i]);
      }
    }
  }
}

// prints polygon data to the stream designated by the FILE pointer.
void polygon_print(Polygon *p, FILE *fp){
  int i;
  
  if(p){
    fprintf(fp, "Polygon:\n");
    if(p->vertex){
      for(i = 0; i < p->nVertex; i++){
        fprintf(fp, "(%f,%f,%f) , ", p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2]);
      }
      fprintf(fp, "\n");
    }
    else{
      fprintf(fp, "no verticies\n");
    }
  }
  else{
    fprintf(fp, "no polygon\n");
  }
}

// draw the outline of the polygon using color c.
void polygon_draw(Polygon *p, Image *src, Color c){
  int i;
  Line edge;
  
  if(p){
    if(p->vertex){
      for(i = 1; i < p->nVertex; i++){
        line_set(&edge, p->vertex[i-1],  p->vertex[i]);
        line_zBuffer(&edge, p->zBuffer);
        line_draw(&edge, src, c);
      }
      line_set(&edge, p->vertex[0],  p->vertex[p->nVertex - 1]);
      line_zBuffer(&edge, p->zBuffer);
      line_draw(&edge, src, c);
    }
  }
}

// set s and t values for texture mapping
void polygon_setST(Polygon *p, int index, float s, float t){
  if ((s > 1.0 || t > 1.0) || (s < 0 || t < 0)){
    printf("!!BAD!! s and t need to be between 0 and 1 !!BAD!!\n");
    return;
  }
  if (index >= p->nVertex){
    printf("index out of range\n");
    return;
  }
  if (!p->s){
    p->s = malloc(sizeof(float)*p->nVertex);
  }
  if (!p->t){
    p->t = malloc(sizeof(float)*p->nVertex);
  }
  p->s[index] = s;
  p->t[index] = t;
}

// draw the filled polygon using color c with the scanline rendering algorithm.
void polygon_drawFill(Polygon *p, Image *src, Color c, int dsFlag){
  if (dsFlag != 0){
    p->zBuffer = 1;
  }
  scanline_drawFill(p, src, c, dsFlag, NULL);
}

void polygon_drawTexture(Polygon *p, Image *src, Mipmap *mipmap){
  Color c;
  if (p->zBuffer == 0 ){
    p->zBuffer = 1;
  }
  scanline_drawFill(p, src, c, 3, mipmap);
}

// draw the filled polygon using color c with the scanline rendering algorithm.
void polygon_drawFillwithTexture(Polygon *p, Image *src, Color *c){
  scanline_drawFill2(p, src, c);
}


// draw the filled polygon using color c with the Barycentric coordinates
// algorithm. Will draw for the zero case. scale makes futher from center less bright
void polygon_drawFillB(Polygon *p, Image *src, Color c){
  float a, b, g, dXab, dYab, dXac, dYac, xyabDiff, xyacDiff, abDenom, acDenom,
    epsilon, Xa, Xb, Xc, Ya, Yb, Yc, scale;
  int maxX, minY, maxY,  i, j;
  
  if(p){
    if(p->nVertex == 3){
            Xa = p->vertex[0].val[0];
            Ya = p->vertex[0].val[1];
            Xb = p->vertex[1].val[0];
            Yb = p->vertex[1].val[1];
            Xc = p->vertex[2].val[0];
            Yc = p->vertex[2].val[1];
            
            maxX = (int)(Xa) + 1;
            if( ((int)(Xb) + 1) > maxX ){
              maxX = (int)(Xb) + 1;
      }
            if( ((int)(Xc) + 1) > maxX ){
              maxX = (int)(Xc) + 1;
      }

            i = (int)(Xa);
      if( (int)(Xb) < i ){
        i = (int)(Xb);
      }
            if( (int)(Xc) < i ){
        i = (int)(Xc);
      }

      maxY = (int)(Ya) + 1;
            if( ((int)(Yb) + 1) > maxY ){
              maxY = (int)(Yb) + 1;
      }
            if( ((int)(Yc) + 1) > maxY ){
              maxY = (int)(Yc) + 1;
      }

            minY = (int)(Ya);
      if( (int)(Yb) < minY ){
        minY = (int)(Yb);
      }
            if( (int)(Yc) < minY ){
        minY = (int)(Yc);
      }

      if( maxX > i && maxY > minY ){
      // if this is false something about our bounding box is wrong

        epsilon = -.00001; // closer than this we consider zero
        
        //calculating peices for beta and gama that are not point dependent
        dXab = Xb - Xa;
        dYab = Ya - Yb;
        dXac = Xa - Xc;
        dYac = Yc - Ya;
        
        xyabDiff = (Xa * Yb) - (Xb * Ya);
        
        xyacDiff = (Xc * Ya) - (Xa * Yc);
        
        abDenom = (dYab * Xc) + (dXab * Yc) + xyabDiff;
        
        acDenom = (dYac * Xb) + (dXac * Yb) + xyacDiff;

        for(; i<maxX; i++){ // loop through bounding box
          for(j = minY; j<maxY; j++){
            b = ( (dYac * (i + 0.5)) + (dXac * (j + 0.5)) + xyacDiff)
            / acDenom;
            if( b > epsilon ){
              g = ( (dYab * (i + 0.5)) + (dXab * (j + 0.5))
              + xyabDiff) / abDenom;
              if( g > epsilon ){
                a = 1.0 - b - g;
                if( a > epsilon ){
                  if( !( (i<0 || j<0)||(src->rows <= i || src->cols <= j) )) {
                    scale = 1; //(a*b*g + 0.02) / .058; 
                    // set to 1 for normal coloring
                    src->data[j][i].rgb[0] = (c.c[0] * scale);
                    src->data[j][i].rgb[1] = (c.c[1] * scale);
                    src->data[j][i].rgb[2] = (c.c[2] * scale);
                  }
                }
              }
            }
          }
        }
      }
      else{
        printf("bonding box error:\nminX:%i, maxX:%i, minY:%i, maxY:%i\n", i, maxX, minY, maxY);
      }
    }
    else{
      printf("Not given a triangle. Given polygon has %i verticies", p->nVertex);
    }
  }
}

// draw the filled polygon using color c with the Barycentric coordinates
// algorithm. Will draw for the zero case. uses a texture from a 8by8 color array
void polygon_drawFillBwithTexture(Polygon *p, Image *src, Color *c){
  float a, b, g, dXab, dYab, dXac, dYac, xyabDiff, xyacDiff, abDenom, acDenom,
    epsilon, Xa, Xb, Xc, Ya, Yb, Yc;
  int maxX, minY, maxY,  i, j;
  
  if(p){
    if(p->nVertex == 3){
            Xa = p->vertex[0].val[0];
            Ya = p->vertex[0].val[1];
            Xb = p->vertex[1].val[0];
            Yb = p->vertex[1].val[1];
            Xc = p->vertex[2].val[0];
            Yc = p->vertex[2].val[1];
            
            maxX = (int)(Xa) + 1;
            if( ((int)(Xb) + 1) > maxX ){
              maxX = (int)(Xb) + 1;
      }
            if( ((int)(Xc) + 1) > maxX ){
              maxX = (int)(Xc) + 1;
      }

            i = (int)(Xa);
      if( (int)(Xb) < i ){
        i = (int)(Xb);
      }
            if( (int)(Xc) < i ){
        i = (int)(Xc);
      }

      maxY = (int)(Ya) + 1;
            if( ((int)(Yb) + 1) > maxY ){
              maxY = (int)(Yb) + 1;
      }
            if( ((int)(Yc) + 1) > maxY ){
              maxY = (int)(Yc) + 1;
      }

            minY = (int)(Ya);
      if( (int)(Yb) < minY ){
        minY = (int)(Yb);
      }
            if( (int)(Yc) < minY ){
        minY = (int)(Yc);
      }

      if( maxX > i && maxY > minY ){
      // if this is false something about our bounding box is wrong

        epsilon = -.00001; // closer than this we consider zero
        
        //calculating peices for beta and gama that are not point dependent
        dXab = Xb - Xa;
        dYab = Ya - Yb;
        dXac = Xa - Xc;
        dYac = Yc - Ya;
        
        xyabDiff = (Xa * Yb) - (Xb * Ya);
        
        xyacDiff = (Xc * Ya) - (Xa * Yc);
        
        abDenom = (dYab * Xc) + (dXab * Yc) + xyabDiff;
        
        acDenom = (dYac * Xb) + (dXac * Yb) + xyacDiff;

        for(; i<maxX; i++){ // loop through bounding box
          for(j = minY; j<maxY; j++){
            b = ( (dYac * (i + 0.5)) + (dXac * (j + 0.5)) + xyacDiff)
            / acDenom;
            if( b > epsilon ){
              g = ( (dYab * (i + 0.5)) + (dXab * (j + 0.5))
              + xyabDiff) / abDenom;
              if( g > epsilon ){
                a = 1.0 - b - g;
                if( a > epsilon ){
                  image_setBitmap(src, j, i, Ya, Xa, c);
                }
              }
            }
          }
        }
      }
            else{
              printf("bonding box error:\nminX:%i, maxX:%i, minY:%i, maxY:%i\n", i, maxX, minY, maxY);
            }
    }
    else{
      printf("Not given a triangle. Given polygon has %i verticies", p->nVertex);
    }
  }
}

void polygon_normalize(Polygon *p){
  int i;
  for (i=0; i<p->nVertex; i++){
    point_normalize(&p->vertex[i]);
  }
}

// sets the oneSided field to the value
void polygon_setSided(Polygon *p, int oneSided){
  p->oneSided = oneSided;
}

// initializes the normal array to the vectors in nlist
void polygon_setNormals(Polygon *p, int numV, Vector *nlist){
  int i;
  
  if(p->nVertex == numV){
    if(p->normal){
      free(p->normal);
    }
    p->normal = malloc(sizeof(Vector) * numV);
    for(i=0; i<numV; i++){
      p->normal[i] = nlist[i];
    }
  }
}
