// hierarchy.c
// Authors: Chris Burnham and Victoria Tisdale
// Created: 10.22.14

#include "list.h"

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
  e->objType = type;
  if (type == ObjNone){
    // do stuff??
  }
  else if (type == ObjLine){
    Line *line = obj;
    e->obj = line
  }
  else if (type == ObjPoint){
    Point *point = obj;
    e->obj = point;
  }
  else if (type == ObjPolyline){
    Polyline *polyline = obj;
    e->obj = polyline;
  }
  else if (type == ObjPolygon){
    Polygon *polygon = obj;
    e->obj = polygon;
  }
  else if (type == ObjIdentity){
    Matrix *matrix = obj;
    e->obj = matrix;
  }
  else if (type == ObjMatrix){
    Matrix *matrix = obj;
    e->obj = matrix;
  }
  else if (type == ObjColor){
    Color *color = obj;
    e->obj = color;
  }
  else if (type == ObjBodyColor){
    Color *color = obj;
    e->obj = color;
  }
  else if (type == ObjSurfaceColor{
    Color *color = obj;
    e->obj = color;
  }
  else if (type == ObjSurfaceCoeff){
    float *coeff = obj;
    e->obj = coeff;
  }
  else if (type == ObjLight){
    // do other stuff
  }
  else if (type == ObjModule){
    e->obj = obj;
  }
}

// free the element and the object it contains, as appropriate.
void element_delete(Element *e){
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