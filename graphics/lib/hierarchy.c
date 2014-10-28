// hierarchy.c
// Authors: Chris Burnham and Victoria Tisdale
// Created: 10.22.14

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
  e = malloc(sizeof(Element));

  switch(type) {
    case ObjNone{
      // do stuff??
			e->obj = NULL;
      break
    }
    case ObjLine {
      Line line;
			line_copy(&line, (Line*)obj);
      e->obj = line
      break;
    }
    case ObjPoint {
      Point point;
			point_copy(&point, (Point*)obj);
      e->obj = point;
      break;
    }
    case ObjPolyline {
      Polyline polyline;
			polyline_init(&polyline);
			polyline_copy(&polyline, (Polyline*)obj);
      e->obj = polyline;
      break;
    }
    case ObjPolygon {
      Polygon polygon;
			polygon_init(&polygon);
			polygon_copy(&polygon, (Polygon*)obj):
      e->obj = polygon;
      break;
    }
    case ObjIdentity {
      e->obj = NULL;
      break;
    }
    case ObjMatrix {
      Matrix matrix;
			matrix_copy(&matrix, (Matrix*)obj);
      e->obj = matrix;
      break;
    }
    case ObjColor {
      Color color;
			color_copy(&color, (Color*)obj);
      e->obj = color;
      break;
    }
    case ObjBodyColor {
      Color color;
			color_copy(&color, (Color*)obj);
      e->obj = color;
      break;
    }
    case ObjSurfaceColor {
      Color color;
			color_copy(&color, (Color*)obj);
      e->obj = color;
      break;
    }
    case ObjSurfaceCoeff {
      float coeff = (float)obj;
      e->obj = coeff;
      break;
    }
    case ObjLight {
      // do other stuff
			e->obj = NULL;
      break;
    }
    case ObjModule {
      e->obj = obj;
      break;
    }
  }
  e->objType = type;
	e->next = NULL;
}

// free the element and the object it contains, as appropriate.
void element_delete(Element *e){
	if(e->objType == Polyline){
		polyline_clear(e->obj.polyline);
	}
	else if(e->objType == Polygon){
		polygon_clear(e->obj.polygon);
	}
  free (e->obj);
  free (e);
}

// Allocate an empty module.
Module *module_create(){
  Module *mod;
  mod = malloc(sizeof(Module));
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
  md->tail.next = e;
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
  Matrix *m;
  matrix_identity(m);
  e = element_init(ObjIdentity, m);
  module_insert(md, e);
}

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty){
  Element *e;
  Matrix *m;
  matrix_translate2D(m);
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy){
  Element *e;
  Matrix *m;
  matrix_scale2D(m);
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a rotation about the Z axis to the tail of the module’s
// list
void module_rotateZ(Module *md, double cth, double sth){
  Element *e;
  Matrix *m;
  matrix_rotateZ(m);
  e = element_init(ObjMatrix, m);
  module_insert(md, e);
}

// Matrix operand to add a 2D shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy){
  Element *e;
  Matrix *m;
  matrix_shear2D(m);
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

  matrix_identity(LTM);
  e = md->head;
  while(e != NULL){
    switch(e->type) {
      case ObjNone {
        // do stuff??
				break;
      }
      case ObjLine {
        Line *line;
        line_copy(line, &e->obj.line);
        matrix_xformLine(LTM, line);
        matrix_xformLine(GTM, line);
        matrix_xformLine(VTM, line);
        line_draw(line, src, ds->color);
      }
      case ObjPoint {
				Point *point;
        point_copy(point, &e->obj.point);
        matrix_xformPoint(LTM, point);
        matrix_xformPoint(GTM, point);
        matrix_xformPoint(VTM, point);
        point_draw(point, src, ds->color);
        break;
      }
      case ObjPolyline {
				Polyline *polyline;
				polyline_init(polyline);
        polyline_copy(polyline, &e->obj.polyline);
        matrix_xformPolyline(LTM, polyline);
        matrix_xformPolyline(GTM, polyline);
        matrix_xformPolyline(VTM, polyline);
        polyline_draw(polyline, src, ds->color);
				polyline_clear(polyline);
        break;
      }
      case ObjPolygon {
				Polygon *polygon;
				polygon_init(polygon);
        polygon_copy(polygon, &e->obj.polygon);
        matrix_xformPolygon(LTM, polygon);
        matrix_xformPolygon(GTM, polygon);
        matrix_xformPolygon(VTM, polygon);

				switch(ds->shade){
					case ShadeFrame {
						polygon_draw(polygon, src, ds->color);
						break;
					}
					case ShadeConstant { //might be supposed to use ds->color
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					}
					case ShadeDepth { //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					}
					case ShadeDepth { //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					}
					case ShadeFlat { //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					}
					case ShadeGouraud { //needs to be changed
						polygon_drawFill(polygon, src, ds->flatColor);
						break;
					}
					// where optional ShadePhong would go
				}
				polygon_clear(polygon);
        break;
      }
      case ObjIdentity {
				matrix_identity(LTM);
        break;
      }
      case ObjMatrix {
				Matrix *matrix;
				matrix_copy(matrix, &e->obj.matrix);
				matrix_multiply(LTM, matrix, LTM);
        break;
      }
      case ObjColor {
				Color *color;
				color_copy(color, &e->obj.color);
				ds->color = color;
        break;
      }
      case ObjBodyColor {
				Color *color;
				color_copy(color, &e->obj.color);
				ds->body = color;
        break;
      }
      case ObjSurfaceColor {
				Color *color;
				color_copy(color, &e->obj.color);
				ds->flatColor = color;
        break;
      }
      case ObjSurfaceCoeff {
				ds->surfaceCoeff = e->obj.coeff;
        break;
      }
      case ObjLight {
        // do other stuff
        break;
      }
      case ObjModule {
				Matrix *newGTM;
				DrawState *newds;
				matrix_multiply(GTM, LTM, newGTM);
				drawstate_copy(newds, ds);
				module_draw(e->obj, VTM, newGTM, newds, lighting, src);
        break;
      }      
    }
    e = e->next;
  }
}

/* 3D Module Functions */

// Matrix operand to add a 3D translation to the Module
void module_translate(Module *md, double tx, double ty, double tz){
	Element *e;
	Matrix *m;
	int i;
	for(i=0; i<4; i++){
		m->m[0][i] = m->m[0][i] + m->m[3][i] * tx;
		m->m[1][i] = m->m[1][i] + m->m[3][i] * ty;
  	m->m[2][i] = m->m[2][i] + m->m[3][i] * tz;
  }
	e = element_create();
	element_init(Matrix, m);
	module_insert(md, m);
}

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz){
	Element *e;
	Matrix *m;
	int i;
	for(i=0; i<4; i++){
		m->m[0][i] = m->m[0][i] * sx;
		m->m[1][i] = m->m[1][i] * sy;
		m->m[2][i] = m->m[2][i] * sz;
  }
	e = element_create();
	element_init(Matrix, m);
	module_insert(md, m);	
}

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth){
	Element *e;
	Matrix *m;
	int i;
	double tmp;
	for(i=0; i<4; i++){
		tmp = m->m[1][i]*cth - m->m[2][i]*sth;
		m->m[2][i] = m->m[1][i]*sth + m->m[2][i]*cth;
		m->m[1][i] = tmp;
  }
	e = element_create();
	element_init(Matrix, m);
	module_insert(md, m);	
}

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth){
	Element *e;
	Matrix *m;
	int i;
	double tmp;
	for(i=0; i<4; i++){
		tmp = m->m[0][i]*cth + m->m[2][i]*sth;
		m->m[2][i] = m->m[2][i]*cth - m->m[0][i]*sth;
		m->m[0][i] = tmp;
  }
	e = element_create();
	element_init(Matrix, m);
	module_insert(md, m);	
}

// Matrix operand to add a rotation that orients to the orthonormal axes u,v,w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
	Element *e;
	Matrix *m;
	int i;
	double tmp0, tmp1;
	for(i=0; i<4; i++){
		tmp0 = m->m[0][i]*u->val[0] + m->m[1][i]*u->val[1] + m->m[2][i]*u->val[2];
		tmp1 = m->m[0][i]*v->val[0] + m->m[1][i]*v->val[1] + m->m[2][i]*v->val[2];
		m->m[2][i] = m->m[0][i]*w->val[0] + m->m[1][i]*w->val[1] + m->m[2][i]*w->val[2];
		m->m[0][i] = tmp0;
		m->m[1][i] = tmp1;
  }
	e = element_create();
	element_init(Matrix, m);
	module_insert(md, m);	
}

// Adds a unit cube, axis-aligned and centered on zero to the Module. If solid is zero, add only lines. If solid is non-zero, use polygons.
void module_cube(Module *md, int solid);

/* Shading/Color Module Functions */

// Adds the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c);

/* DrawState Functions */

// create a new DrawState structure and initialize the fields
DrawState *drawstate_create(void);

// set the color field to c
void drawstate_setColor(DrawState *s, Color c);

// set the body field to c
void drawstate_setBody(DrawState *s, Color c);

// set the surface field to c
void drawstate_setSurface(DrawState *s, Color c);

// set the surfaceCoeff field to f
void drawstate_setSurfaceCoeff( DrawState *s, float f);

// copy the DrawState data
void drawstate_copy(DrawState *to, DrawState *from);
