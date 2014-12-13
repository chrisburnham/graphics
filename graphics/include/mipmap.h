// mipmap.h

#ifndef MIPMAP_H
#define MIPMAP_H

#include "image.h"

typedef union{
  float c;
  float rgb[3];
} CValue;

typedef struct{
  CValue **data;
  int dim; // dimension
  float level; // log(d)
} Mipmap;

Mipmap *mipmap_create();
void mipmap_free(Mipmap *src);
int mipmap_fill(Image *src, Mipmap *dst);

#endif
