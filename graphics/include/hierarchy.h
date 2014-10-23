// hierarchy.h
// Authors: Chris Burnham and Victoria Tisdale
// Created: 10.22.14

#ifndef HIERARCHY_H
#define HIERARCHY_H

typedef enum { // example of an enumerated type
  ObjNone,
  ObjLine,
  ObjPoint,
  ObjPolyline,
  ObjPolygon,
  ObjIdentity,
  ObjMatrix,
  ObjColor,
  ObjBodyColor,
  ObjSurfaceColor,
  ObjSurfaceCoeff,
  ObjLight,
  ObjModule
} ObjectType;

typedef union {
  Point point;
  Line line;
  Polyline polyline;
  Polygon polygon;
  Matrix matrix;
  Color color;
  float coeff;
  void *module;
} Object;

// Module structure
typedef struct {
  ObjectType type;
  Object obj;
  void *next;
} Element;

typedef struct {
  Element *head; // pointer to the head of the linked list
  Element *tail; // keep around a pointer to the last object
} Module;

// Allocate and return an initialized but empty Element.
Element *element_create();

// Allocate an Element and store a duplicate of the data pointed to by obj in 
// the Element. Modules do not get duplicated. The function needs to handle each
// type of object separately in a case statement.
Element *element_init(ObjectType type, void *obj);

// free the element and the object it contains, as appropriate.
void element_delete(Element *e);

// Allocate an empty module.
Module *module_create();

// clear the module’s list of Elements, freeing memory as appropriate.
void module_clear(Module *md);

// Free all of the memory associated with a module, including the memory pointed to by md.
void module_delete(Module *md);

// Generic insert of an element into the module at the tail of the list.
void module_insert(Module *md, Element *e);

// Adds a pointer to the Module sub to the tail of the module’s list.
void module_module(Module *md, Module *sub);

// Adds p to the tail of the module’s list.
void module_point(Module *md, Point *p);

// Adds p to the tail of the module’s list.
void module_line(Module *md, Line *p);

// Adds p to the tail of the module’s list.
void module_polyline(Module *md, Polyline *p);

// Adds p to the tail of the module’s list.
void module_polygon(Module *md, Polygon *p);

// Object that sets the current transform to the identity, placed at the tail of
// the module’s list.
void module_identity(Module *md);

// Matrix operand to add a translation matrix to the tail of the module’s list.
void module_translate2D(Module *md, double tx, double ty);

// Matrix operand to add a scale matrix to the tail of the module’s list.
void module_scale2D(Module *md, double sx, double sy);

// Matrix operand to add a rotation about the Z axis to the tail of the module’s
// list
void module_rotateZ(Module *md, double cth, double sth);

// Matrix operand to add a 2D shear matrix to the tail of the module’s list.
void module_shear2D(Module *md, double shx, double shy);

#endif