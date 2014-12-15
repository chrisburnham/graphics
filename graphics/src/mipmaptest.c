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
    Image *dst;
    int i, j;
    Color c;

    // src = image_create(256, 256);
    dst = image_create(512, 512);
    src = image_read("colors.ppm");
    
    mip = mipmap_create();
    mipmap_fill(src, mip);
    // mipmap_print(mip, stdout);
    
    for(i=0; i<512; i++){ // rows
        for (j=0; j<512; j++){ // cols
            if (i<256 && j<256){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<256){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <256){
                color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<384 && j<384){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<384){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <384){
                color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<448 && j<448){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<448){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <448){
                color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<480 && j<480){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<480){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <480){
                color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<496 && j<496){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<496){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <496){
               color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<504 && j<504){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<504){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <504){
               color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<508 && j<508){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<508){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <508){
               color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<510 && j<510){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<510){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <510){
               color_set(&c, 0, 0, mip->data[i][j]);
            }
            else if (i<511 && j<511){
                color_set(&c, mip->data[i][j], 0, 0);
            }
            else if (i<511){
                color_set(&c, 0, mip->data[i][j], 0);
            }
            else if (j <511){
               color_set(&c, 0, 0, mip->data[i][j]);
            }
            image_setColor(dst, i, j, c);
        }
    }

    image_write(dst, "mipmap.ppm");
    system("convert mipmap.ppm mipmap.png");
    system("rm mipmap.ppm");
    image_free(dst);
    mipmap_free(mip);
    image_free(src);
    return(0);
}