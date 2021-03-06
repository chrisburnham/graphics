// hierarchy.h
// Authors: Chris Burnham and Victoria Tisdale
// Created: 10.22.14

#ifndef HIERARCHY_H
#define HIERARCHY_H

typedef enum {
    LightNone,
    LightAmbient,
    LightDirect,
    LightPoint,
    LightSpot,
} LightType;

typedef struct {
    LightType type;
    Color color;
    Vector direction;
    Point position;
    float cutoff; // stores the cosine of the cutoff angle of a spotlight
    float sharpness; // coefficient of the falloff function (power for cosine)
} Light;

typedef struct{
    int nLights;
    Light light[64];
} Lighting;

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
    Light light;
  float coeff;
  void *module; //don't know if we can make this not a void pointer
} Object;

// Module structure
typedef struct Anode{
  ObjectType type;
  Object obj;
  struct Anode*next; //should work instead of void pointer
} Element;

typedef struct {
  Element *head; // pointer to the head of the linked list
  Element *tail; // keep around a pointer to the last object
} Module;

typedef enum {
	ShadeFrame,			// draw only the borders of objects, including polygons
	ShadeConstant,	// draw objects using the current foreground color, fill polygons
	ShadeDepth,			// draw objects using the current foreground color, fill polygons
	ShadeFlat,			// draw objects using shading calculations, but each polygon is a constant value
	ShadeGouraud,		// draw objects using Gouraud shading
  ShadeMipmap         // draws using the mipmap
	// optional ShadePhong would go here
} ShadeMethod;

typedef struct {
  Color color;			// the foreground color, used in the default drawing mode
	Color flatColor;		// the color to flat-fill a polygon based on a shading calculation
	Color body;				// the body reflection color, used for shading calculations
	Color surface;		// the surface reflection color, used for shading calculations
	float surfaceCoeff;//a float that represents the shininess of the surface
	ShadeMethod shade;// an enumerated type ShadeMethod
	int zBufferFlag;	// whether to use z-buffer hidden surface removal
	Point viewer;			// a  Point representing the view location in 3D (identical to the VRP in View3D)
	Mipmap mipmap;       // mipmap holding the texture
} DrawState;


/* 2D and Generic Module Functions */

// Allocate and return an initialized but empty Element.
Element *element_create(void);

// Allocate an Element and store a duplicate of the data pointed to by obj in 
// the Element. Modules do not get duplicated. The function needs to handle each
// type of object separately in a case statement.
Element *element_init(ObjectType type, void *obj);

// free the element and the object it contains, as appropriate.
void element_delete(Element *e);

// Allocate an empty module.
Module *module_create(void);

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

// Draw the module into the image using the given view transformation matrix 
// [VTM], Lighting and DrawState by traversing the list of Elements. (For now, 
// Lighting can be an empty structure.)
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, 
    Lighting *lighting, Image *src);

// does the lighting pass. should be called before module draw
void module_lighting(Module *md, Matrix *VTM, Matrix *GTM, Lighting *lighting );


/* 3D Module Functions */

// Matrix operand to add a 3D translation to the Module
void module_translate(Module *md, double tx, double ty, double tz);

// Matrix operand to add a 3D scale to the Module
void module_scale(Module *md, double sx, double sy, double sz);

// Matrix operand to add a rotation about the X-axis to the Module
void module_rotateX(Module *md, double cth, double sth);

// Matrix operand to add a rotation about the Y-axis to the Module
void module_rotateY(Module *md, double cth, double sth);

// Matrix operand to add a rotation that orients to the orthonormal axes u,v,w
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);

// Adds a unit cube, axis-aligned and centered on zero to the Module. 
// If solid is zero, add only lines. If solid is non-zero, use polygons. 
// Make sure each polygon has surface normals defined for it.
void module_cube(Module *md, int solid);

// Adds a unit cylinder Bruce Maxwells function
void module_cylinder(Module *mod, int sides);

//Adds a unit sphere 
void module_sphere(Module *mod);

/* Shading/Color Module Functions */

// Adds the foreground color value to the tail of the module's list
void module_color(Module *md, Color *c);

// Adds the body color value to the tail of the module's list
void module_bodyColor(Module *md, Color *c);

// Adds the surface color value to the tail of the module's list
void module_surfaceColor(Module *md, Color *c);

//Adds the specular coefficeient to the tail of the module's list
void module_surfaceCoeff(Module *md, float coeff);

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

/* Light Functions */

// initialize the light to default values
void light_init( Light *light );

// copy the light information
void light_copy( Light *to, Light *from );

/* Lighting Functions */

// allocate and return a new lighting structure set to default values
Lighting *lighting_create( void );

// initialize the lighting structure to default values
void lighting_init( Lighting *l );

//add a new light to the Lighting structure given the parameters, some of which may be NULL, depending upon the type. Make sure you don’t add more lights than MAX LIGHTS.
void lighting_add( Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness );

// calculate the proper color given the normal N, view vector V, 3D point P, body color Cb, surface color Cs, sharpness value s, the lighting, and whether the polygon is one-sided or two-sided. Put the result in the Color c.
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c );


#endif
