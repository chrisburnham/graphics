/* data stuctures and function headers for objects
 Chris Burnham
 */

#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct{
    double m[4][4];
} Matrix;

typedef Point Vector;


/*Vector Functions*/

//set the Vector to (x,y,z,0.0)
void vector_set(Vector *v, double x, double y, double z);

//print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp);

//returns the Euclidean length of the vector, assuming the homogeneous coordinate is 1.0
double vector_length(Vector *v);

//normalize the Vector to unit length. Do not modify the homogeneous coordinate
void vector_normalize(Vector *v);

//returns the scalar product of a and b
double vector_dot(Vector *a, Vector *b);

//calculates the cross product of a and b and puts the result in c
void vector_cross(Vector *a, Vector *b, Vector *c);


/*2D and Generic Matrix Functions*/

//print out the matrix in a nice 4X4 arrangement with a blank line below
void matrix_print(Matrix *m, FILE *fp);

//set the matrix to all zeros
void matrix_clear(Matrix *m);

//set the matrix to the idenity matrix
void matrix_identity(Matrix *m);

//return the element of the matrix at row r, column c
double matrix_get(Matrix *m, int r, int c);

//set the element of the matrix at row r, column c to v
void matrix_set(Matrix *m, int r, int c, double v);

//copy the src matrix into the dest matrix
void matrix_copy(Matrix *dest, Matrix *src);

//transpose the matrix m in place
void matrix_transpose(Matrix *m);

//multiply left and right and put the result in m. The result matrix can also be the left or right matrix.
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m);

//transforms the point p by the matrix m and put the result in q. For this function, p and q need to be diffrent variables
void matrix_xformPoint(Matrix *m, Point *p, Point *q);

//transform the vector p by the matrix m and put the result in q. For this function, p and q need to be different variables
void matrix_xformVector(Matrix *m, Vector *p, Vector *q);

//transform the points (and surface normals (non existant)) in the polygon p by the matrix m
void matrix_xformPolygon(Matrix *m, Polygon *p);

//transform the points in the polyline p by the matrix m
void matrix_xformPolyline(Matrix *m, Polyline *p);

//transform the points in line by the matrix m
void matrix_xformLine(Matrix *m, Line *line);

//premultiply the matrix by a scale matrix parameterized by sx and sy
void matrix_scale2D(Matrix *m, double sx, double sy);

//premultiply the matrix by z-axis rotation matrix parameterized by cos and sin of the angle of rotation about the z-axis
void matrix_rotateZ(Matrix *m, double cth, double sth);

//premultiply the matrix by a 2D traslation matrix parameterized by tx and ty
void matrix_translate2D(Matrix *m, double tx, double ty);

//premultiply the matrix by a 2D shear matrix parameterized by shx and shy
void matrix_shear2D(Matrix *m, double shx, double shy);




#endif