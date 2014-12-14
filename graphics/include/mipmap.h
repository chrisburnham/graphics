// mipmap.h

#ifndef MIPMAP_H
#define MIPMAP_H

#include "image.h"

//  mipmap format
//  rrrrgggg
//  rrrrgggg
//  rrrrgggg
//  rrrrgggg
//  bbbb
//  bbbb    rrgg
//  bbbb    rrgg
//  bbbb    bb
//          bb  rg
//              b

typedef struct{
  float data[512][512];
} Mipmap;

Mipmap *mipmap_create(void);
void mipmap_free(Mipmap *src);
int mipmap_fill(Image *src, Mipmap *dst);
void mipmap_print(Mipmap *src, FILE *fp);

#endif
