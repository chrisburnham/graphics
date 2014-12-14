// mipmap.c

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mipmap.h"
#include "graphics.h"



//  returns empty mipmap structure
//  currently all mipmaps:
//  dem = 512
//  l = 9

Mipmap *mipmap_create(){
  Mipmap *mipmap;
  
  mipmap = malloc(sizeof(Mipmap));
  return( mipmap );
}

void mipmap_free(Mipmap *src){
  if(src){
    free(src);
  }
}


// fills the mipmap with values 
// starting from the top left pixel of the Image
// images must be larger than 256 by 256 (only takes first 256 by 256)
int mipmap_fill(Image *src, Mipmap *dst){
  int i, j, k, pow2, sum0, sum1, sum2;
    printf("image size: %i, %i\n", src->rows, src->cols);
  if (src->rows < 256 || src->cols < 256){
    printf("Image too small for mipmap\n");
    return (-1);
  }
    
    for(i=0; i<256; i++){
        for(j=0; j<256; j++){
            dst->data[i][j] = src->data[i][j].rgb[0];
            dst->data[i][j+256] = src->data[i][j].rgb[1];
            dst->data[i+256][j] = src->data[i][j].rgb[2];
        }
    }
    
    pow2 = 1;
    sum0 = 0;
    sum1 = 256;
    for(k=0; k<7; k++){
        pow2 = pow2 * 2;
        sum2 = sum1 + (256/pow2);
        
        for(i=0; i<(256/pow2); i++){
            for(j=0; j<(256/pow2); j++){
                dst->data[i+sum1][j+sum1] = (dst->data[2*i+sum0][2*j+sum0] + dst->data[2*i+sum0+1][2*j+sum0] + dst->data[2*i+sum0][2*j+sum0+1] + dst->data[2*i+sum0+1][2*j+sum0+1]) / 4;
                dst->data[i+sum1][j+sum2] = (dst->data[2*i+sum0][2*j+sum1] + dst->data[2*i+sum0+1][2*j+sum1] + dst->data[2*i+sum0][2*j+sum1+1] + dst->data[2*i+sum0+1][2*j+sum1+1]) / 4;
                dst->data[i+sum2][j+sum1] = (dst->data[2*i+sum1][2*j+sum0] + dst->data[2*i+sum1+1][2*j+sum0] + dst->data[2*i+sum1][2*j+sum0+1] + dst->data[2*i+sum1+1][2*j+sum0+1]) / 4;
            }
        }
        sum0 = sum1;
        sum1 = sum2;
    }
    
  /*
  // first 3 blocks of color
  for (i=255; i>=0; i++){
    for (j=255; j>=0; j++){
      dst->data[i][j].c = src->data[i][j].rgb[0]; // red values
      dst->data[i][j+256].c = src->data[i][j].rgb[1]; // green values
      dst->data[i+256][j+256].c = src->data[i][j].rgb[2]; // blue values
      // need to code in averages
      // if (i < 128 && j < 128){
      //   dst->data[i+256][j] = src->data.rgb[0]
      //   dst->data[i+256][j+128] = src->data.rgb[1];
      //   dst->data[i+384][j+128] = src->data.rgb[2];
      // }
      // if (i<64 && j<64){
      //   dst->data[i+384][j] = src->data.rgb[0]
      //   dst->data[i+384][j+64] = src->data.rgb[1];
      //   dst->data[i+448][j+64] = src->data.rgb[2];
      // }
      // if (i<32 && j<32){
      //   dst->data[i+448][j] = src->data.rgb[0]
      //   dst->data[i+448][j+32] = src->data.rgb[1];
      //   dst->data[i+480][j+32] = src->data.rgb[2];
      // }
      // if (i<16 && j<16){
      //   dst->data[i+480][j] = src->data.rgb[0]
      //   dst->data[i+480][j+16] = src->data.rgb[1];
      //   dst->data[i+496][j+16] = src->data.rgb[2];
      // }
      // if (i<8 && j<8){
      //   dst->data[i+496][j] = src->data.rgb[0]
      //   dst->data[i+496][j+8] = src->data.rgb[1];
      //   dst->data[i+504][j+8] = src->data.rgb[2];
      // }
      // if (i<4 && j<4){
      //   dst->data[i+504][j] = src->data.rgb[0]
      //   dst->data[i+504][j+4] = src->data.rgb[1];
      //   dst->data[i+508][j+4] = src->data.rgb[2];
      // }
      // if (i<2 && j<2){
      //   dst->data[i+508][j] = src->data.rgb[0]
      //   dst->data[i+508][j+2] = src->data.rgb[1];
      //   dst->data[i+510][j+2] = src->data.rgb[2];
      // }
      // if (i<1 && j<1){
      //   dst->data[511][511] = src->data.rgb
      // }
    }
  }*/
  return (0);
}

void mipmap_print(Mipmap *src, FILE *fp){
    int i, j;
    fprintf(fp, "Mipmap:\n");
    for(i=0; i<512; i++){
        for(j=0; j<512; j++){
            fprintf(fp, "%i ", (int)src->data[i][j]);
        }
        fprintf(fp, "\n");
    }
}

