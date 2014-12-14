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
  Module *cube;
  int rows = 360;
  int cols = 640;
  Mipmap *mipmap;
  Image *texture;

  Color White;
  Color Grey;

  DrawState *ds;
  View3D view;

  Lighting *light;

  color_set( &White, 1.0, 1.0, 1.0 );
  color_set( &Grey, 0.6, 0.62, 0.64 );

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

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
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  // make a simple cube module
  cube = module_create();
  module_scale( cube, 3, 1, 2 );

  // this would color the cube in ShadeConstant mode
  module_color( cube, &Grey );

  // the example cube is blue (Y/-Y), red (Z/-Z), yellow (X/-X)
  // these colors should be the body colors
  module_cube( cube, 1);

  // manually add a light source to the Lighting structure
  // put it in the same place as the eye in world space
  light = lighting_create();
  lighting_add( light, LightPoint, &White, NULL, &(view.vrp), 0, 0 );

  // set the shading to Gouraud
  ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp));
  ds->shade = ShadeMipmap;
  //  ds->shade = ShadeFlat;
  texture = image_read("starfuries.ppm");
  mipmap = mipmap_create();
  mipmap_fill(texture, mipmap);
    printf("llama\n");
  ds->mipmap = *mipmap;
printf("llama0\n");
  matrix_identity(&GTM);
    printf("llama1\n");
  module_draw(cube, &VTM, &GTM, ds, light, src);
    printf("llama2\n");

  // write out the image
  image_write(src, "test9a.ppm");

  // free stuff here
  module_delete( cube );
  image_free( src );
  

  return(0);
}