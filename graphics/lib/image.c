// Image.c

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "image.h"
#include "ppmIO.h"

Image *image_create(int rows, int cols){
	Image *image;
	int i;
	
	image = malloc(sizeof(Image));
	image->rows = rows;
	image->cols = cols;
	
	if( rows < 1 || cols < 1 ){
		image->data = NULL;	
	}
	else{
		image->data = malloc(sizeof(FPixel *) * rows);
		image->data[0] = malloc(sizeof(FPixel) * rows * cols);
		for(i=1; i<rows; i++){
				image->data[i] = &( image->data[0][i*cols] );
		}	
	}
  image_reset(image);
	return( image );
}

void image_free(Image *src){
	if(src){
		image_dealloc(src);
		free(src);
	}
}

void image_init(Image *src){
	src->rows = 0;
	src->cols = 0;
	src->data = NULL;
}

int image_alloc(Image *src, int rows, int cols){
	int i;
	
	if(src){
		if(src->data){
			if(src->data[0]){
				free(src->data[0]);
			}
			free(src->data);
			src->data = NULL;		
		}
		src->data = malloc(sizeof(FPixel *) * rows);
		src->data[0] = malloc(sizeof(FPixel) * rows * cols);
		for(i=1; i<rows; i++){
				src->data[i] = &( src->data[0][i*cols] );
		}		

		src->rows = rows;
		src->cols = cols;
		for(i=0; i<(rows*cols); i++){
			src->data[0][i].rgb[0] = 0.0;
			src->data[0][i].rgb[0] = 0.0;
			src->data[0][i].rgb[0] = 0.0;
			src->data[0][i].a = 0.0;
			src->data[0][i].z = 1.0;
		}
		return( 0 );
	}
	else{
		return( 1 );
	}
}

void image_dealloc(Image *src){
	if(src){	
		if(src->data){
			if(src->data[0]){
				free(src->data[0]);
			}
			free(src->data);
		}
		src->data = NULL;
		src->rows = 0;
		src->cols = 0;
	}
}

Image *image_read(char *filename){
	Pixel *input;
	Image *image;
	int i, rows, cols, colors;

	input = readPPM(&rows, &cols, &colors, filename);
	image = image_create(rows,cols);
	image->rows = rows;
	image->cols = cols;

	for(i=0; i<(rows*cols); i++){
		image->data[0][i].rgb[0] = (float)input[i].r / 255.0;
		image->data[0][i].rgb[1] = (float)input[i].g / 255.0;
		image->data[0][i].rgb[2] = (float)input[i].b / 255.0;
		image->data[0][i].a = 1.0;
		image->data[0][i].z = 1.0;
	}
	free(input);
	return( image );
}

int image_write(Image *src, char *filename){
	Pixel *output;
	int i;
	float r, g, b;

	if(src){
		output = malloc(sizeof(Pixel)*(src->rows)*(src->cols));
		for(i=0; i<(src->rows * src->cols); i++){
			/* this is where you can choose to do something other than truncating */
			r = src->data[0][i].rgb[0];
			if(r > 1){
				r = 1;
			}
			else if(r < 0){
				r = 0;
			}
			r = r * 255;

			g = src->data[0][i].rgb[1];
			if(g > 1){
				g = 1;
			}
			else if(g < 0){
				g = 0;
			}
			g = g * 255;

			b = src->data[0][i].rgb[2];
			if(b > 1){
				b = 1;
			}
			else if(b < 0){
				b = 0;
			}
			b = b * 255;

			output[i].r = (int)r;
			output[i].g = (int)g;
			output[i].b = (int)b;
		}
	
		writePPM(output, src->rows, src->cols, 255, filename);
		free(output);
		return( 0 );
	}
	else{
		return( 1 );
	}
}

FPixel image_getf(Image *src, int r, int c){
	return( src->data[r][c] );
}

float image_getc(Image *src, int r, int c, int b){
	return( src->data[r][c].rgb[b] );
}

float image_geta(Image *src, int r, int c){
	return( src->data[r][c].a );
}

float image_getz(Image *src, int r, int c){
	if( (r<0 || c<0)||(src->rows <= r || src->cols <= c)) {
		return( 1.0 );
	}
	else{
		return( src->data[r][c].z );
	}
}

void image_setf(Image *src, int r, int c, FPixel val){
	src->data[r][c] = val;
}

void image_setc(Image *src, int r, int c, int b, float val){
	src->data[r][c].rgb[b] = val;
}

void image_seta(Image *src, int r, int c, float val){
	src->data[r][c].a = val;
}

void image_setz(Image *src, int r, int c, float val){
	if( (r<0 || c<0)||(src->rows <= r || src->cols <= c)) {
		return;
	}
	else{
		src->data[r][c].z = val;
	}
}

void image_reset(Image *src){
	FPixel normPix;	

	normPix.rgb[0] = 0.0;
	normPix.rgb[1] = 0.0;		
	normPix.rgb[2] = 0.0;
	normPix.a = 1.0;
	normPix.z = 1.0;

	image_fill(src, normPix);
}

void image_fill(Image *src, FPixel val){
	int i, j;

	if(src){
		for(i=0; i<(src->rows); i++){
			for(j=0; j<(src->cols); j++){
				image_setf(src, i, j, val);
			}
		}
	}
}

void image_fillrgb(Image *src, float r, float g, float b){
	int i;
	
	if(src){
		for(i=0; i<(src->rows * src->cols); i++){
			src->data[0][i].rgb[0] = r;
			src->data[0][i].rgb[1] = g;
			src->data[0][i].rgb[2] = b;
		}
	}
}

void image_filla(Image *src, float a){
	int i;
	
	if(src){
		for(i=0; i<(src->rows * src->cols); i++){
			src->data[0][i].a = a;
		}
	}
}

void image_fillz(Image *src, float z){
	int i;
	
	if(src){
		for(i=0; i<(src->rows * src->cols); i++){
			src->data[0][i].z = z;
		}
	}
}

//copies the Color data to the proper pixel
void image_setColor(Image *src, int r, int c, Color val){
	if( (r<0 || c<0)||(src->rows <= r || src->cols <= c)) {
		return;
	}
	else{
		src->data[r][c].rgb[0] = val.c[0];
		src->data[r][c].rgb[1] = val.c[1];
		src->data[r][c].rgb[2] = val.c[2];
	}
}

//returns a Color structure built from the pixel values
Color image_getColor(Image *src, int r, int c){
	Color color;

	color.c[0] = image_getc(src, r, c, 0);
	color.c[1] = image_getc(src, r, c, 1);
	color.c[2] = image_getc(src, r, c, 2);

	return color;
}

//colors based off of a 8 by 8 bit map passed in as a array of colors
void image_setBitmap(Image *src, int r, int c, int fixR, int fixC, Color *vals){	
	src->data[r][c].rgb[0] = vals[8*((r-fixR)%8) + (c-fixC)%8].c[0];
	src->data[r][c].rgb[1] = vals[8*((r-fixR)%8) + (c-fixC)%8].c[1];
	src->data[r][c].rgb[2] = vals[8*((r-fixR)%8) + (c-fixC)%8].c[2];
}

void squareNoise(Image *src, int num){
	int rows, cols, i, j, k, max_size, x, y, size;
	float dim;

	if(src){
		if(src->data){
			if(src->data[0]){
				rows = src->rows;
				cols = src->cols;
				
				max_size = 100;
				if( (max_size * 10) > rows ){
					max_size = rows / 10 ;
				}
				if( (max_size * 10) > cols ){	
					max_size = cols / 10;
				}
				
				for(i=0; i<num; i++){
					size = ( rand() % max_size ) + 10; //note the actual max size is 10 greater than max_size it is just for convience
					x = rand() % (rows - size);
					y = rand() % (cols - size);
					dim = ((float)(rand() % 100) / 200) + 0.5;

					for(j=0; j<size; j++){
						for(k=0; k<size; k++){
							src->data[x+j][y+k].rgb[0] = (src->data[x+j][y+k].rgb[0] * dim);
							src->data[x+j][y+k].rgb[1] = (src->data[x+j][y+k].rgb[1] * dim);
							src->data[x+j][y+k].rgb[2] = (src->data[x+j][y+k].rgb[2] * dim);
						}
					}
				}		
			}
		}
	}
}

/* Color functions */
//copies the Color data
void color_copy(Color *to, Color *from){
	*to = *from;
}

//sets the Color data
void color_set(Color *to, float r, float g, float b){
	to->c[0] = r;
	to->c[1] = g;
	to->c[2] = b;
}




























