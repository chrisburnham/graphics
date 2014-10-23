// Project 3 (parts 4 and 5)
// Victoria Tisdale

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "graphics.h"

void setWhite( Image *src );
void setWhite( Image *src ) {
  int i, j;
  Color White;

  color_set(&White, 1.0, 1.0, 1.0);

  for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
      image_setColor( src, i, j, White );
    }
  }
}

int main(int argc, char *argv[]) {
  const int rows = 100;
  const int cols = 100;
  const int nFrames = 16;
  // View3D view3D;
  View2D view;
  //Matrix vtm;
  Matrix vtm, gtm, ltm;
  Polygon poly[16];
  Point vp[4];
  FILE *fp;

  fp = fopen("matrix_info.txt","w");

  Image *src;
  int i, j, t;
  char filename[256];

  Color color[6];
  
  // set some colors
  color_set( &color[0], 0, 0, 1 ); // blue
  color_set( &color[1], 0, 1, 0 ); // green
  color_set( &color[2], 1, 0, 0 ); // red
  color_set( &color[3], 1, 0, 1 ); // magenta
  color_set( &color[4], 0, 1, 1 ); // cyan
  color_set( &color[5], 1, 1, 0 ); // yellow

  // optional theta value
  // if(argc > 1) {
  //   theta = atoi(argv[1]);
  // }

  // initialize the three matrices
  matrix_identity(&vtm);
  matrix_identity(&gtm);
  matrix_identity(&ltm);
  // create image
  src = image_create( rows, cols );

  srand ( time(NULL) );

  for (i=0; i<16; i++){
    for (j=0; j<4; j++){
      point_set2D(&(vp[j]), rand()%cols, rand()%rows);
    }
    poly[i] = *(polygon_createp(4, vp));
  }

  // grab command line argument to determine viewpoint
  // and set up the view structure
  if( argc > 1 ) {
    float alpha = atof( argv[1] );
    if( alpha < 0.0 || alpha > 1.0 ){
      alpha = 0.0;
    }
    point_set1( &(view.vrp), 3*alpha, 2*alpha, -2*alpha - (1.0-alpha)*3 );
  }
  else {
    point_set1( &(view.vrp), 3, 2, -2 );
  }

  vector_set( &(view.x), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
  view.dx = 1;  // focal length
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView2D( &vtm, &view );
  matrix_print(&vtm, fp);
  // create image in arbitrary world coordinates
   
  for(t=0;t<nFrames;t++) {
    
    setWhite( src );

    for (i=0; i<16; i++){
      // need to add print statements to follow the ltm and polygons
      // Polygons not drawing
      matrix_identity(&ltm);
      matrix_translate2D(&ltm, -vp[0].val[0], -vp[0].val[1]);
      matrix_shear2D(&ltm, t, 0);
      matrix_translate2D(&ltm, vp[0].val[0], vp[0].val[1]);
      matrix_xformPolygon(&ltm, &poly[i]);
      matrix_xformPolygon(&vtm, &poly[i]);
      polygon_drawFill(&poly[i], src, color[i%5]);
    }

    sprintf(filename, "test5vt-%04d.ppm", t );
    image_write( src, filename );

    // translate the view across the scene
    point_set2D( &(view.vrp), 1.8 - 2.4*(t+1)/nFrames, 1.8 - 2.4*(t+1)/nFrames );
    matrix_setView2D( &vtm, &view );
  }
  fclose(fp);
  system("convert test5vt-*.ppm test5vt.gif");
  //system("rm test5vt-*.ppm");
}