// Project 3 (parts 4 and 5)
// Victoria Tisdale

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"


int main(int argc, char *argv[]) {
  const int rows = 300;
  const int cols = 300;
  const int nFrames = 100;
  View3D view;
  Matrix vtm;
  Matrix t;
  Polygon side[6];
  Polygon tpoly;
  Point  tc;
  Point  v[8];
  Circle c;
  Color  color[6];
  Image *src;
  int i, t;
  char filename[256];
  
  color[0].c[0]=1.0;
  color[0].c[2]=1.0;
  color[0].c[3]=1.0;
  
  point_set2D(tc, 150, 150);
  cirlce_set(c, tc, 100);
  cirlce_draw(src, c, color[0])
  // need to create lsys for polygon
  // for i to 10;




  // edit array of polygon vertexs and edges
  // going to create one polygon that is the tree
  // can then loop over the list of vertex and transform to create gif
}