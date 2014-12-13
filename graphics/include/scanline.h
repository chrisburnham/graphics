/*
  Bruce A. Maxwell
  Fall 2014

  Skeleton scanline fill algorithm

  * edits by Victoria Tisdale *
*/

#include "image.h"
#include "polygon.h"
#include "objects.h"
#include "mipmap.h"

void scanline_drawFill(Polygon *p, Image *src, Color c, int dsFlag, Mipmap *mipmap) ;
void scanline_drawFill2(Polygon *p, Image *src, Color *c) ;