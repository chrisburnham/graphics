// hierarchy.c
// Authors: Chris Burnham and Victoria Tisdale
// Created: 10.22.14

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include "list.h"

/* 2D and Generic Module Functions */

// Allocate and return an initialized but empty Element.
Element *element_create(){
  Element *e;
  e = malloc(sizeof(Element));
  return e;
}

// Allocate an Element and store a duplicate of the data pointed to by obj in 
// the Element. Modules do not get duplicated. The function needs to handle each
// type of object separately in a case statement.
Element *element_init(ObjectType type, void *obj){
  Element *e;
	Line line;
	Point point;
	Polyline polyline;
	Polygon polygon;
	Matrix matrix;
	Color color;
	float *coeff;
			

	polyline_init(&polyline);
	polygon_init(&polygon);
  e = malloc(sizeof(Element));

  switch(type) {
    case ObjNone:
      // do stuff??
			//e->obj = NULL;
      break;
    
    case ObjLine:
			line_copy(&line, (Line*)obj);
      e->obj.line = line;
      break;
    
    case ObjPoint:
			point_copy(&point, (Point*)obj);
      e->obj.point = point;
      break;
    
    case ObjPolyline:
			polyline_copy(&polyline, (Polyline*)obj);
      e->obj.polyline = polyline;
      break;
    
    case ObjPolygon:
			polygon_copy(&polygon, (Polygon*)obj);
      e->obj.polygon = polygon;
      break;
    
    case ObjIdentity:
      //e->obj = NULL;
      break;
    
    case ObjMatrix:
			matrix_copy(&matrix, (Matrix*)obj);
      e->obj.matrix = matrix;
      break;
    
    case ObjColor:
			color_copy(&color, (Color*)obj);
      e->obj.color = color;
      break;
    
    case ObjBodyColor:
			color_copy(&color, (Color*)obj);
      e->obj.color = color;
      break;
    
    case ObjSurfaceColor:
			color_copy(&color, (Color*)obj);
      e->obj.color = color;
      break;
    
    case ObjSurfaceCoeff:
      coeff = (float*)obj;
      e->obj.coeff = *coeff;
      break;
    
    case ObjLight:
      // do other stuff
			//e->obj = NULL;
      break;
    
    case ObjModule:
      e->obj.module = obj;
      break;
    
  }
	polyline_clear(&polyline);
	polygon_clear(&polygon);
  e->type = type;
	e->next = NULL;
}

// free the element and the object it contains, as appropriate.
void element_delete(Element *e){
	if(e->type == ObjPolyline){
		polyline_clear(&(e->obj.polyline));
	}
	else if(e->type == ObjPolygon){
		polygon_clear(&(e->obj.polygon));
	}
  free (e);
}

// Allocate an empty module.
Module *module_create(){
  Module *mod;
  mod = malloc(sizeof(Module));
	return mod;
}

// clear the module’s list of Elements, freeing memory as appropriate.
void module_clear(Module *md){
  Element *e;
      e = md->head;
  while (e->next != NULL){ 
    md->head = e->next;
    element_delete(e);
    e = md->head;
  }
  free(e);
  md->head = NULL;
  md->tail = NULL;
}

// Free all of the memory associated with a module, including the memory pointed to by md.
void module_delete(Module *md){
  module_clear(md);
  free(md->head);
  free(md->tail);
  free(md);
}

// Generic insert of an element into the module at the tail of the list.
void module_insert(Module *md, Element *e){
  md->tail->next = e;
  md->tail = e;
}

// Adds a pointer to the Module sub to the tail of the module’s list.
void module_module(Module *md, Module *sub){
  Element *e;
  e = element_init(ObjModule, sub);
  module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_point(Module *md, Point *p){
  Element *e;
  e = element_init(ObjPoint, p);
  module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_line(Module *md, Line *p){
  Element *e;
  e = element_init(ObjLine, p);
  module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_polyline(Module *md, Polyline *p){
  Element *e;
  e = element_init(ObjPolyline, p);
  module_insert(md, e);
}

// Adds p to the tail of the module’s list.
void module_polygon(Module *md, Polygon *p){
  Element *e;
  e = element_init(ObjPolygon, p);
  module_insert(md, e);
}

// Object that sets the current transform to the identity, placed at the tail of
// the module’s list.
void module_identity(Module *md){
  Element *e;
  e = element_init(ObjIdentity, NULL);
  module_insert(md, e);
}

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty){
  Element *e;
  Matrix *m;
  matrix_identity(m);
	m->m[0][3] = tx;
	m->m[1][3] = ty;
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy){
  Element *e;
  Matrix *m;
  matrix_identity(m);
	m->m[0][0] = sx;
	m->m[1][1] = sy;
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a rotation about the Z axis to the tail of the module’s
// list
void module_rotateZ(Module *md, double cth, double sth){
  Element *e;
  Matrix *m;
  matrix_identity(m);
	m->m[0][0] = cth;
	m->m[1][1] = cth;
	m->m[0][1] = -sth;
	m->m[1][0] = sth;
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a 2D shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy){
  Element *e;
  Matrix *m;
  matrix_identity(m);
	m->m[0][1] = shx;
	m->m[1][0] = shy;
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Draw the module into the image using the given view transformation matrix 
// [VTM], Lighting and DrawState by traversing the list of Elements. (For now, 
// Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, 
    Lighting *lighting, Image *src){
  Element *e;
  Matrix *LTM;
	Line *line;
	Point *point;
	Polyline *polyline;
	Polygon *polygon;
	Matrix *matrix;
	Matrix *newGTM;
	DrawState *newds;

	polygon_init(polygon);
	polyline_init(polyline);
  matrix_identity(LTM);
  e = md->head;
  while(e != NULL){
    switch(e->type) {
      case ObjNone:
        // do stuff??
				break;
      
      case ObjLine:
        line_copy(line, &(e->obj.line));
        matrix_xformLine(LTM, line);
        matrix_xformLine(GTM, line);
        matrix_xformLine(VTM, line);
        line_draw(line, src, ds->color);
      
      case ObjPoint:
        matrix_xformPoint(LTM, &(e->obj.point), point);
        matrix_xformPoint(GTM, &(e->obj.point), point);
        matrix_xformPoint(VTM, &(e->obj.point), point);
        point_draw(point, src, ds->color);
        break;
      
      case ObjPolyline:
        polyline_copy(polyline, &(e->obj.polyline));
        matrix_xformPolyline(LTM, polyline);
        matrix_xformPolyline(GTM, polyline);
        matrix_xformPolyline(VTM, polyline);
        polyline_draw(polyline, src, ds->color);
        break;
      
      case ObjPolygon:
        polygon_copy(polygon, &(e->obj.polygon));
        matrix_xformPolygon(LTM, polygon);
        matrix_xformPolygon(GTM, polygon);
        matrix_xformPolygon(VTM, polygon);

				switch(ds->shade){
					case ShadeFrame:
						polygon_draw(polygon, src, ds->color);
						break;
					
					case ShadeConstant: //might be supposed to use ds->color
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					
					case ShadeDepth: //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					
					case ShadeFlat: //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					
					case ShadeGouraud: //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					
					// where optional ShadePhong would go
				}
        break;
      
      case ObjIdentity:
				matrix_identity(LTM);
        break;
      
      case ObjMatrix:
				matrix_copy(matrix, &(e->obj.matrix));
				matrix_multiply(LTM, matrix, LTM);
        break;
      
      case ObjColor:
				color_copy(&(ds->color), &(e->obj.color));
        break;
      
      case ObjBodyColor:
				color_copy(&(ds->body), &(e->obj.color));
        break;
      
      case ObjSurfaceColor:
				color_copy(&(ds->flatColor), &(e->obj.color));
        break;
      
      case ObjSurfaceCoeff:
				ds->surfaceCoeff = e->obj.coeff;
        break;
      
      case ObjLight:
        // do other stuff
        break;
      
      case ObjModule:
				matrix_multiply(GTM, LTM, newGTM);
				drawstate_copy(newds, ds);
				module_draw(e->obj.module, VTM, newGTM, newds, lighting, src);
        break;
           
    }
    e = e->next;
  }
	polyline_clear(polyline);
	polygon_clear(polygon);
}

/* 3D Module Functions */

// Matrix operand to add a 3D translation to the Module
void module_translate(Module *md, double tx, double ty, double tz){
	Element *e;
	Matrix *m;
	matrix_identity(m);
	m->m[0][3] = tx;
	m->m[1][3] = ty;
	m->m[2][3] = tz;
	e = element_init(ObjMatrix, m);
	module_insert(md, e);
}

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz){
	Element *e;
	Matrix *m;
	matrix_identity(m);
	m->m[0][0] = sx;
	m->m[1][1] = sy;
	m->m[2][2] = sz;
	e = element_init(ObjMatrix, m);
	module_insert(md, e);	
}

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth){
	Element *e;
	Matrix *m;
	matrix_identity(m);
	m->m[1][1] = cth;
	m->m[1][2] = -sth;
	m->m[2][1] = sth;
	m->m[2][2] = cth;
	e = element_init(ObjMatrix, m);
	module_insert(md, e);	
}

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth){
	Element *e;
	Matrix *m;
	matrix_identity(m);
	m->m[0][0] = cth;
	m->m[0][2] = sth;
	m->m[2][0] = -sth;
	m->m[2][2] = cth;
	e = element_create();
	element_init(ObjMatrix, m);
	module_insert(md, e);	
}

// Matrix operand to add a rotation that orients to the orthonormal axes u,v,w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
	Element *e;
	Matrix *m;\
	int i;
	matrix_identity(m);
	for(i=0; i<3; i++){
		m->m[0][i] = u->val[i];
		m->m[1][i] = v->val[i];
		m->m[2][i] = w->val[i];
	}
	e = element_create();
	element_init(ObjMatrix, m);
	module_insert(md, e);	
}

// Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines. If solid is non-zero, use polygons.
void module_cube(Module *md, int solid){
	Point vtex[8];
	Line edge[12];
	Polygon side[6];
	Point square[4];
	Element *e;
	int i;

	point_set3D(&(vtex[0]),0,0,0);
	point_set3D(&(vtex[1]),0,0,1);
	point_set3D(&(vtex[2]),0,1,0);
	point_set3D(&(vtex[3]),0,1,1);
	point_set3D(&(vtex[4]),1,0,0);
	point_set3D(&(vtex[5]),1,0,1);
	point_set3D(&(vtex[6]),1,1,0);
	point_set3D(&(vtex[7]),1,1,1);

	if(solid == 0){
		line_set(&(edge[0]), vtex[0], vtex[1]);
		line_set(&(edge[1]), vtex[0], vtex[2]);
		line_set(&(edge[2]), vtex[0], vtex[4]);
		line_set(&(edge[3]), vtex[1], vtex[3]);
		line_set(&(edge[4]), vtex[1], vtex[5]);
		line_set(&(edge[5]), vtex[2], vtex[3]);
		line_set(&(edge[6]), vtex[2], vtex[6]);
		line_set(&(edge[7]), vtex[3], vtex[7]);
		line_set(&(edge[8]), vtex[4], vtex[5]);
		line_set(&(edge[9]), vtex[4], vtex[6]);
		line_set(&(edge[10]), vtex[5], vtex[7]);
		line_set(&(edge[11]), vtex[6], vtex[7]);

		for(i=0; i<12; i++){
			e = element_create();
			element_init(ObjLine, &(edge[i]));
			module_insert(md, e);
		}
	}
	else{
		for(i=0; i<6; i++){
			polygon_init(&(side[i]));
		}
		point_copy(&(square[0]), &(vtex[0]));
		point_copy(&(square[1]), &(vtex[1]));
		point_copy(&(square[2]), &(vtex[3]));
		point_copy(&(square[3]), &(vtex[2]));
		polygon_set(&side[0], 4, square);

		point_copy(&(square[0]), &(vtex[0]));
		point_copy(&(square[1]), &(vtex[1]));
		point_copy(&(square[2]), &(vtex[5]));
		point_copy(&(square[3]), &(vtex[4]));
		polygon_set(&side[1], 4, square);

		point_copy(&(square[0]), &(vtex[0]));
		point_copy(&(square[1]), &(vtex[2]));
		point_copy(&(square[2]), &(vtex[6]));
		point_copy(&(square[3]), &(vtex[4]));
		polygon_set(&side[2], 4, square);

		point_copy(&(square[0]), &(vtex[1]));
		point_copy(&(square[1]), &(vtex[3]));
		point_copy(&(square[2]), &(vtex[7]));
		point_copy(&(square[3]), &(vtex[5]));
		polygon_set(&side[3], 4, square);

		point_copy(&(square[0]), &(vtex[2]));
		point_copy(&(square[1]), &(vtex[3]));
		point_copy(&(square[2]), &(vtex[7]));
		point_copy(&(square[3]), &(vtex[6]));
		polygon_set(&side[0], 4, square);

		point_copy(&(square[0]), &(vtex[4]));
		point_copy(&(square[1]), &(vtex[5]));
		point_copy(&(square[2]), &(vtex[7]));
		point_copy(&(square[3]), &(vtex[6]));
		polygon_set(&side[5], 4, square);

		for(i=0; i<6; i++){
			e = element_create();
			element_init(ObjPolygon, &(side[i]));
			module_insert(md, e);
		}
	}
}

/* Shading/Color Module Functions */

// Adds the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c){
	Element *e;
	e = element_create();
	element_init(ObjColor, c);
	module_insert(md, e);
}

/* DrawState Functions */

// create a new DrawState structure and initialize the fields
DrawState *drawstate_create(void){
	DrawState *ds;
	ds = malloc(sizeof(DrawState));
	return ds;
}

// set the color field to c
void drawstate_setColor(DrawState *s, Color c){
	s->color = c;
}

// set the body field to c
void drawstate_setBody(DrawState *s, Color c){
	s->body = c;
}

// set the surface field to c
void drawstate_setSurface(DrawState *s, Color c){
	s->surface = c;
}

// set the surfaceCoeff field to f
void drawstate_setSurfaceCoeff( DrawState *s, float f){
	s->surfaceCoeff = f;
}

// copy the DrawState data
void drawstate_copy(DrawState *to, DrawState *from){
	color_copy(&(to->color), &(from->color));
	color_copy(&(to->flatColor), &(from->flatColor));
	color_copy(&(to->body), &(from->body));
	color_copy(&(to->surface), &(from->surface));
	to->surfaceCoeff = from->surfaceCoeff;
	to->shade = from->shade;
	to->zBufferFlag = from->zBufferFlag;
	point_copy(&(to->viewer), &(from->viewer));
}
