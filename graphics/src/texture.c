// Victoria Tisdale
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Matrix LTM;
  Module *cube;
  int rows = 360;
  int cols = 640;
  Mipmap *mipmap;
  Image *texture;

  Color White;
  Color Grey;
  Color Black;

  DrawState *ds;
  View3D view;

  Lighting *light;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Grey, 0.6, 0.62, 0.64 );
  color_set( &Black, 0.0, 0.0, 0.0 );

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);
  matrix_identity(&LTM);

  // set the View parameters
  point_set3D(&(view.vrp), 5, 5, -7.0);
  vector_set(&(view.vpn), -5, -5, 7);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.6;
  view.dv = .9;
  view.f = 0.0;
  view.b = 15;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  // printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  module_scale( cube, 1, 2, 2);

  // this would color the cube in ShadeConstant mode
  module_color(cube, &Grey);

  // the example cube is blue (Y/-Y), red (Z/-Z), yellow (X/-X)
  // these colors should be the body colors
  module_cube(cube, 1);
  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );

  // set the shading to Gouraud
  ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp));
  ds->shade = ShadeMipmap;
  //  ds->shade = ShadeFlat;
  // ds->shade = ShadeGouraud;
  printf("hello\n");
  texture = image_read("colors.ppm");
  
  mipmap = mipmap_create();
  printf("rows: %d\ncols: %d\n", texture->rows, texture->cols);
  mipmap_fill(texture, mipmap);
  ds->mipmap = *mipmap;
  // drawstate_setColor( ds, Grey );
  // drawstate_setBody( ds, Black);
  // drawstate_setSurface( ds, Grey);

  matrix_rotateY(&GTM, -1, 0);
  matrix_rotateX(&GTM, 0, 1);
  matrix_rotateZ(&GTM, 0, 1);
  module_lighting(cube, &VTM, &GTM, light );
  module_draw(cube, &VTM, &GTM, ds, light, src);
  printf("Victoria\n");
  
  image_write(src, "texture.ppm");
  system ("convert texture.ppm texture.png");
  system ("rm texture.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );
  

  return(0);
}