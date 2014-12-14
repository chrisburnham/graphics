/*
 Chris Burnham
 11/8/14
 Jetman animation
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

int main(int argc, char *argv[]) {
    Image *src;
    Mipmap *mip;
    
    src = image_create(256, 256);
    image_fillrgb(src, 0, 1, 5);
    
    mip = mipmap_create();
    mipmap_fill(src, mip);
    mipmap_print(mip, stdout);
    
    mipmap_free(mip);
    image_free(src);
    return(0);
}