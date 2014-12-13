// mipmap.c

#include "mipmap.h"


//
//  returns empty mipmap structure
//  currently all mipmaps:
//  dem = 512
//  l = 9
//
Mipmap *mipmap_create(){
  
  Mipmap *mipmap;
  int i;
  
  mipmap = malloc(sizeof(Image));
  mipmap->dim = 512;
  
  if( size < 1 ){
    mipmap->data = NULL; 
  }
  else{
    mipmap->data = malloc(sizeof(CValue *) * 512);
    mipmap->data[0] = malloc(sizeof(CValue) * 512 * 512);
    for(i=1; i<512; i++){
        mipmap->data[i] = &( mipmap->data[0][i*512] );
    } 
  }
  return( mipmap );
}

void mipmap_free(mipmap *src){
  if(src){
    image_dealloc(src);
    free(src);
  }
}

//
// fills the mipmap with values 
// starting from the top left pixel of the Image
//
int mipmap_fill(Image *src, Mipmap *dst){
  int i, j;
  if (src->rows != src->cols && src->rows != 265){
    printf("incorrect input image for mipmap v00\n");
    return (-1);
  }
  // first 3 blocks of color
  for (i=0; i<256; i++){
    for (j=0; j<(dst->256); j++){
      if (i<1 && j<1){
        dst->data[511][511] = src->data.rgb
      }
      if (i<2 && j<2){
        dst->data[i+508][j] = src->data.rgb[0]
        dst->data[i+508][j+2] = src->data.rgb[1];
        dst->data[i+510][j+2] = src->data.rgb[2];
      }
      if (i<4 && j<4){
        dst->data[i+504][j] = src->data.rgb[0]
        dst->data[i+504][j+4] = src->data.rgb[1];
        dst->data[i+508][j+4] = src->data.rgb[2];
      }
      if (i<8 && j<8){
        dst->data[i+496][j] = src->data.rgb[0]
        dst->data[i+496][j+8] = src->data.rgb[1];
        dst->data[i+504][j+8] = src->data.rgb[2];
      }
      if (i<16 && j<16){
        dst->data[i+480][j] = src->data.rgb[0]
        dst->data[i+480][j+16] = src->data.rgb[1];
        dst->data[i+496][j+16] = src->data.rgb[2];
      }
      if (i<32 && j<32){
        dst->data[i+448][j] = src->data.rgb[0]
        dst->data[i+448][j+32] = src->data.rgb[1];
        dst->data[i+480][j+32] = src->data.rgb[2];
      }
      if (i<64 && j<64){
        dst->data[i+384][j] = src->data.rgb[0]
        dst->data[i+384][j+64] = src->data.rgb[1];
        dst->data[i+448][j+64] = src->data.rgb[2];
      }
      if (i < 128 && j < 128){
        dst->data[i+256][j] = src->data.rgb[0]
        dst->data[i+256][j+128] = src->data.rgb[1];
        dst->data[i+384][j+128] = src->data.rgb[2];
      }
      dst->data[i][j].c = src->data[i][j].rgb[0]; // red values
      dst->data[i][j+256].c = src->data[i][j].rgb[1]; // green values
      dst->data[i+256][j+256].c = src->data[i][j].rgb[2]; // blue values
    }
  }
  return (0);
}

