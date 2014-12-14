/*
 Bruce A. Maxwell
 Fall 2013
 
 Reads data from a PLY file
 
 Returns...
 
 a list of polygons (complete with surface normals)
 a list of colors
 
 Blender can export to PLY files (but doesn't seem to save colors)
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphics.h"

typedef enum {
	type_float32,
	type_uint8,
	type_int32,
	type_list,
	type_none
} ply_type;

typedef struct {
	ply_type type;
	ply_type listCardType;
	ply_type listDataType;
	char name[32];
	void *next;
} ply_property;

ply_type plyType(char *buffer);
ply_type plyType(char *buffer) {
	if(!strcmp(buffer, "float32"))
		return(type_float32);
    
	if(!strcmp(buffer, "uint8"))
		return(type_uint8);
    
	if(!strcmp(buffer, "int32"))
		return(type_int32);
    
	if(!strcmp(buffer, "list"))
		return(type_list);
    
	return(type_none);
}

#define MaxVertices (10)

int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals);
int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals) {
	char buffer[256];
	Point *vertex;
	Vector *normal;
	//  Point *texture;
	Color *color;
	Polygon *p;
	int numPoly;
	int numVertex;
	int vertexProp = 0;
	int faceProp = 0;
	ply_property *vertexproplist = NULL;
	ply_property *vertexproptail = NULL;
	ply_property *faceproplist = NULL;
	ply_property *faceproptail = NULL;
	int nv;
	int vid[MaxVertices];
	int i, j;
	Color tcolor;
    
	// first line ought to be "ply"
	// format ought to be "ascii 1.0"
	// comment lines are ignored
    
	// element creates an element structure and specifies how many
	// vertex is meaningful
	// float32 is %f
	// uint8 is %c
	// int32 is %d
	// x, y, z, nx, ny, nz, s, t, red, green, blue all have meaning
    
	// element face is meaningful
	// property list means # of elements, followed by type
	// vertex_indices means make polygons out of them
    
	// end_header means the first element type starts
    
	int doneWithHeader = 0;
	FILE *fp = fopen(filename, "r");
	if(fp) {
		// check if it's a .ply file
		fscanf(fp, "%s", buffer);
		if(strcmp(buffer, "ply")) {
			printf("%s doesn't look like a .ply file\n", filename);
			fclose(fp);
			return(-1);
		}
        
		while(!doneWithHeader) {
			fscanf(fp, "%s", buffer);
			switch(buffer[0]) {
                case 'f':
                    // format statement
                    for(;fgetc(fp) != '\n';);
                    break;
                    
                case 'c':
                    // comment
                    for(;fgetc(fp) != '\n';);
                    break;
                    
                case 'p':
                    // property statement
                {
                    ply_property *prop = malloc(sizeof(ply_property));
                    prop->listCardType = type_none;
                    prop->listDataType = type_none;
                    prop->next = NULL;
                    
                    fscanf(fp, "%s", buffer); // get the data type
                    prop->type = plyType(buffer);
                    if(prop->type == type_list) {
                        fscanf(fp, "%s", buffer); // get the first data type
                        prop->listCardType = plyType(buffer);
                        fscanf(fp, "%s", buffer); // get the first data type
                        prop->listDataType = plyType(buffer);
                    }
                    else if(prop->type == type_none) {
                        printf("Unrecognized property type %s", buffer);
                        fclose(fp);
                        return(-1);
                    }
                    printf("Read property type %d\n", prop->type);
                    
                    fscanf(fp, "%s", prop->name);
                    printf("Read property name %s\n", prop->name);
                    
                    // add the property entry to the list
                    if(vertexProp) {
                        if(vertexproplist == NULL) {
                            vertexproplist = prop;
                            vertexproptail = prop;
                        }
                        else {
                            vertexproptail->next = prop;
                            vertexproptail = prop;
                        }
                    }
                    else if(faceProp) {
                        if(faceproplist == NULL) {
                            faceproplist = prop;
                            faceproptail = prop;
                        }
                        else {
                            faceproptail->next = prop;
                            faceproptail = prop;
                        }
                    }
                }
                    break;
                    
                case 'e':
                    if(!strcmp(buffer, "end_header")) {
                        doneWithHeader = 1;
                        break;
                    }
                    
                    // otherwise it's an element statement
                    fscanf(fp, "%s", buffer);
                    if(!strcmp(buffer, "vertex")) {
                        printf("Read element vertex\n");
                        vertexProp = 1;
                        faceProp = 0;
                        fscanf(fp, "%d", &numVertex);
                    }
                    else if(!strcmp(buffer, "face")) {
                        printf("Read element face\n");
                        faceProp = 1;
                        vertexProp = 0;
                        fscanf(fp, "%d", &numPoly);
                    }
                    break;
                    
                default: // don't know what to do with it
                    for(;fgetc(fp) != '\n';);
                    break;
			}
		}
		// finished with the header
		vertex = malloc(sizeof(Point) * numVertex);
		normal = malloc(sizeof(Vector) * numVertex);
		// texture
		color = malloc(sizeof(Color) * numVertex); // apparently not written by Blender
        
		// read the vertices
		for(i=0;i<numVertex;i++) {
			for(j=0;j<3;j++)
				fscanf(fp, "%lf", &(vertex[i].val[j]));
			vertex[i].val[3] = 1.0;
            
			for(j=0;j<3;j++)
				fscanf(fp, "%lf", &(normal[i].val[j]));
			normal[i].val[3] = 0.0;
            
			for(j=0;j<2;j++)
				fscanf(fp, "%*f");
            
			for(j=0;j<3;j++) {
				fscanf(fp, "%f", &(color[i].c[j]));
				color[i].c[j] /= 255.0;
			}
		}
        
		p = malloc(sizeof(Polygon) * numPoly);
		*clist = malloc(sizeof(Color) * numPoly);
        
		// read the faces and build the polygons
		for(i=0;i<numPoly;i++) {
			polygon_init(&(p[i]));
            
			// read in the vertex indices
			nv = 0;
			fscanf(fp, "%d", &nv);
            
			if(nv > MaxVertices) {
				printf("Number of vertices is greater than MaxVertices (%d), terminating\n", nv);
				exit(-1);
			}
            
			for(j=0;j<nv;j++) {
				fscanf(fp, "%d", &(vid[j]));
			}
            
			// assign the polygon vertices and surface normals
			// not setting vertexWorld right now, because no Phong shading
            
			p[i].nVertex = nv;
            //			p[i].zBufferFlag = 1;
			p[i].normal = malloc(sizeof(Vector)*nv);
			p[i].vertex = malloc(sizeof(Point)*nv);
			tcolor.c[0] = tcolor.c[1] = tcolor.c[2] = 0.0;
			//      printf("%d: ", nv);
			for(j=0;j<nv;j++) {
				//	printf("%d  ", vid[j]);
				p[i].vertex[j] = vertex[vid[j]];
				if(!estNormals) {
					p[i].normal[j] = normal[vid[j]];
				}
				tcolor.c[0] += color[vid[j]].c[0];
				tcolor.c[1] += color[vid[j]].c[1];
				tcolor.c[2] += color[vid[j]].c[2];
			}
			tcolor.c[0] /= (float)nv;
			tcolor.c[1] /= (float)nv;
			tcolor.c[2] /= (float)nv;
            
			if(estNormals) {
				Vector tx, ty, tn;
                
				tx.val[0] = p[i].vertex[0].val[0] - p[i].vertex[1].val[0];
				tx.val[1] = p[i].vertex[0].val[1] - p[i].vertex[1].val[1];
				tx.val[2] = p[i].vertex[0].val[2] - p[i].vertex[1].val[2];
                
				ty.val[0] = p[i].vertex[2].val[0] - p[i].vertex[1].val[0];
				ty.val[1] = p[i].vertex[2].val[1] - p[i].vertex[1].val[1];
				ty.val[2] = p[i].vertex[2].val[2] - p[i].vertex[1].val[2];
                
				vector_cross(&tx, &ty, &tn);
				vector_normalize(&tn);
                
				for(j=0;j<nv;j++)
					p[i].normal[j] = tn;
			}
            
			printf("(%.2f %.2f %.2f)\n", tcolor.c[0], tcolor.c[1], tcolor.c[2]);
            
			(*clist)[i] = tcolor;
		}
        
		*nPolygons = numPoly;
		*plist = p;
        
		free(vertex);
		free(normal);
		//    free(texture);
		free(color);
        
		{
			ply_property *q;
            
			while(vertexproplist != NULL) {
				q = (ply_property *)vertexproplist->next;
				free(vertexproplist);
				vertexproplist = q;
			}
            
			while(faceproplist != NULL) {
				q = (ply_property *)faceproplist->next;
				free(faceproplist);
				faceproplist = q;
			}
		}
        
		fclose(fp);
	}
	else {
		printf("Unable to open %s\n", filename);
		return(-1);
	}
    
	return(0);
}


/*
 Bruce A. Maxwell
	Fall 2014

	Test program for project 9
 */


int main(int argc, char *argv[]) {
  Image *src;
  Matrix VTM;
  Matrix GTM;
  Module *starfury;
  Module *scene;

  // make it big and shrink the result
  int rows = 2000;
  int cols = 2000;
  int i;

  int nPolygons;
  Polygon *plist;
  Color *clist;

  Color AmbientColor;
  Color PointColor;
  Color PointColor2;
  Color SurfaceColor;

	Point pos;

  Lighting *light;
  DrawState *ds;

  View3D view;

  char buffer[256];

  double angle = 0.0;

	color_set(&AmbientColor, 0.01, 0.01, 0.01);
	color_set(&PointColor, 0.07, 0.06, 0.045);
	color_set(&PointColor2, 0.02, 0.03, 0.045);
	color_set(&SurfaceColor, 0.2, 0.2, 0.2);

  if(argc > 2) {
    angle = atof(argv[2]);
    angle = angle * M_PI / 180.0;
  }

  // initialize the image
  src = image_create(rows, cols);

  // initialize matrices
  matrix_identity(&GTM);
  matrix_identity(&VTM);

  // set the View parameters
  point_set3D(&(view.vrp), 0.0, 0.0, -15.0);
  vector_set(&(view.vpn), 0.0, 0.0, 1.0);
  vector_set(&(view.vup), 0.0, 1.0, 0.0);
  view.d = 2.0;
  view.du = 1.4;
  view.dv = 1.4;
  view.f = 0.0;
  view.b = 100;
  view.screenx = cols;
  view.screeny = rows;
  matrix_setView3D(&VTM, &view);

  // print out VTM
  printf("Final VTM: \n");
  matrix_print(&VTM, stdout);

  readPLY(argv[1], &nPolygons, &plist, &clist, 1);

  printf("Read %d polygons\n", nPolygons);

  starfury = module_create();
  module_surfaceColor(starfury, &SurfaceColor);
  for(i=0;i < nPolygons;i++) {
	  printf("%.2f %.2f %.2f\n", clist[i].c[0], clist[i].c[1], clist[i].c[2] );
      //module_surfaceColor(starfury, &clist[i]);
    module_bodyColor(starfury, &clist[i]);
    module_polygon(starfury, &(plist[i]));
  }

  scene = module_create();
  module_rotateY(scene, cos(angle), sin(angle));
  module_translate(scene, -1.0, -2.0, 0.0);
  module_module(scene, starfury);
  module_translate(scene, 3, 3, 3);
  module_module(scene, starfury);

  light = lighting_create();

  // create one point light
	
	point_set3D( &pos, 0.0, 0.0, -50.0 );
	lighting_add( light, LightPoint, &PointColor, NULL, &pos, 0.0, 0.0 );

  // create another point light
	point_set3D( &pos, 50.0, -20.0, -50.0 );
	lighting_add( light, LightPoint, &PointColor2, NULL, &pos, 0.0, 0.0 );

  // create an ambient light
	lighting_add( light, LightAmbient, &AmbientColor, NULL, NULL, 0.0, 0.0 );

  matrix_identity(&GTM);

  ds = drawstate_create();
  point_copy( &(ds->viewer), &(view.vrp) );
  ds->shade = ShadeGouraud;

  printf("shading frame\n");
    module_lighting(scene, &VTM, &GTM, light);
  module_draw(scene, &VTM, &GTM, ds, light, src);

  // write out the image
  printf("Writing out high resolution image\n");
  sprintf(buffer, "starfuries.ppm");
  image_write(src, buffer);

  printf("Converting to low resolution image\n");
  sprintf(buffer, "convert -resize 25%% starfuries.ppm starfuries.png");
  system(buffer);

  printf("Deleting high-res image\n");
  //  system("rm starfuries.ppm");

  // ought to free stuff here
	image_free(src);

  return(0);
}
