/* matrix function definitions
 Chris Burnham and Victoria Tisdale 11/11
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "objects.h"
#include "polygon.h"
#include "matrix.h"


/*Vector Functions*/

//set the Vector to (x,y,z,0.0)
void vector_set(Vector *v, double x, double y, double z){
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
    v->val[3] = 0.0;
}

//print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp){
    fprintf(fp, "(%f,%f,%f,%f)\n", v->val[0], v->val[1], v->val[2], v->val[3]);
}

//returns the Euclidean length of the vector, assuming the homogeneous coordinate is 1.0
double vector_length(Vector *v){
    return sqrt( v->val[0]*v->val[0] + v->val[1]*v->val[1] + v->val[2]*v->val[2] + v->val[3]*v->val[3] );
}

//normalize the Vector to unit length. Do not modify the homogeneous coordinate
void vector_normalize(Vector *v){
    double length;
    length = sqrt(  v->val[0]*v->val[0] + v->val[1]*v->val[1] + v->val[2]*v->val[2] + v->val[3]*v->val[3] );
    v.val[0] = v.val[0] / length;
    v.val[1] = v.val[1] / length;
    v.val[2] = v.val[2] / length;
    v.val[3] = v.val[3] / length;
}

//returns the scalar product of a and b
double vector_dot(Vector *a, Vector *b){
    return a->val[0]*b->val[0] + a->val[1]*b->val[1] + a->val[2]*b->val[2] + a->val[3]*b->val[3];
}

//calculates the cross product of a and b and puts the result in c
void vector_cross(Vector *a, Vector *b, Vector *c){
    c->val[0] = a->val[1]*b->vla[2] - a->val[2]*b->val[1];
    c->val[1] = a->val[2]*b->vla[0] - a->val[0]*b->val[2];
    c->val[2] = a->val[0]*b->vla[1] - a->val[1]*b->val[0];
}


/*2D and Generic Matrix Functions*/

//print out the matrix in a nice 4X4 arrangement with a blank line below
void matrix_print(Matrix *m, FILE *fp){
    int i;
    for(i = 0; i < 4; i++){
        fprintf(fp, "[%f, %f, %f, %f]\n", m->m[i][0], m->m[i][1], m->m[i][2], m->m[i][3]);
    }
    fprintf(fp, "\n");
}

//set the matrix to all zeros
void matrix_clear(Matrix *m){
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            m->m[i][j] = 0.0;
        }
    }
}

//set the matrix to the idenity matrix
void matrix_identity(Matrix *m){
    m->m[0][0]=1.0; m->m[0][1]=0.0; m->m[0][2]=0.0; m->m[0][3]=0.0;
    m->m[1][0]=0.0; m->m[1][1]=1.0; m->m[1][2]=0.0; m->m[1][3]=0.0;
    m->m[2][0]=0.0; m->m[2][1]=0.0; m->m[2][2]=1.0; m->m[2][3]=0.0;
    m->m[3][0]=0.0; m->m[3][1]=0.0; m->m[3][2]=0.0; m->m[3][3]=1.0;
}

//return the element of the matrix at row r, column c
double matrix_get(Matrix *m, int r, int c){
    return m->m[r][c];
}

//set the element of the matrix at row r, column c to v
void matrix_set(Matrix *m, int r, int c, double v){
    m->m[r][c] = v;
}

//copy the src matrix into the dest matrix
void matrix_copy(Matrix *dest, Matrix *src){
    int i, j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            dest->m[i][j] = src->m[i][j];
        }
    }
}

//transpose the matrix m in place
void matrix_transpose(Matrix *m){
    double tmp;
    tmp = m->m[0][1]; m->m[0][1] = m->m[1][0]; m->m[1][0] = tmp;
    tmp = m->m[0][2]; m->m[0][2] = m->m[2][0]; m->m[2][0] = tmp;
    tmp = m->m[0][3]; m->m[0][3] = m->m[3][0]; m->m[3][0] = tmp;
    tmp = m->m[1][2]; m->m[1][2] = m->m[2][1]; m->m[2][1] = tmp;
    tmp = m->m[1][3]; m->m[1][3] = m->m[3][1]; m->m[3][1] = tmp;
    tmp = m->m[2][3]; m->m[2][3] = m->m[3][2]; m->m[3][2] = tmp;
    
}

//multiply left and right and put the result in m. The result matrix can also be the left or right matrix.
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m){
    Matrix result;
    result[0][0] = (left->m[0][0]*right->m[0][0] + left->m[0][1]*right->m[1][0] + left->m[0][2]*right->m[2][0] + left->m[0][3]*right->m[3][0]);
    result[0][1] = (left->m[0][0]*right->m[0][1] + left->m[0][1]*right->m[1][1] + left->m[0][2]*right->m[2][1] + left->m[0][3]*right->m[3][1]);
    result[0][2] = (left->m[0][0]*right->m[0][2] + left->m[0][1]*right->m[1][2] + left->m[0][2]*right->m[2][2] + left->m[0][3]*right->m[3][2]);
    result[0][3] = (left->m[0][0]*right->m[0][3] + left->m[0][1]*right->m[1][3] + left->m[0][2]*right->m[2][3] + left->m[0][3]*right->m[3][3]);
    result[1][0] = (left->m[1][0]*right->m[0][0] + left->m[1][1]*right->m[1][0] + left->m[1][2]*right->m[2][0] + left->m[1][3]*right->m[3][0]);
    result[1][1] = (left->m[1][0]*right->m[0][1] + left->m[1][1]*right->m[1][1] + left->m[1][2]*right->m[2][1] + left->m[1][3]*right->m[3][1]);
    result[1][2] = (left->m[1][0]*right->m[0][2] + left->m[1][1]*right->m[1][2] + left->m[1][2]*right->m[2][2] + left->m[1][3]*right->m[3][2]);
    result[1][3] = (left->m[1][0]*right->m[0][3] + left->m[1][1]*right->m[1][3] + left->m[1][2]*right->m[2][3] + left->m[1][3]*right->m[3][3]);
    result[2][0] = (left->m[2][0]*right->m[0][0] + left->m[2][1]*right->m[1][0] + left->m[2][2]*right->m[2][0] + left->m[2][3]*right->m[3][0]);
    result[2][1] = (left->m[2][0]*right->m[0][1] + left->m[2][1]*right->m[1][1] + left->m[2][2]*right->m[2][1] + left->m[2][3]*right->m[3][1]);
    result[2][2] = (left->m[2][0]*right->m[0][2] + left->m[2][1]*right->m[1][2] + left->m[2][2]*right->m[2][2] + left->m[2][3]*right->m[3][2]);
    result[2][3] = (left->m[2][0]*right->m[0][3] + left->m[2][1]*right->m[1][3] + left->m[2][2]*right->m[2][3] + left->m[2][3]*right->m[3][3]);
    result[3][0] = (left->m[3][0]*right->m[0][0] + left->m[3][1]*right->m[1][0] + left->m[3][2]*right->m[2][0] + left->m[3][3]*right->m[3][0]);
    result[3][1] = (left->m[3][0]*right->m[0][1] + left->m[3][1]*right->m[1][1] + left->m[3][2]*right->m[2][1] + left->m[3][3]*right->m[3][1]);
    result[3][2] = (left->m[3][0]*right->m[0][2] + left->m[3][1]*right->m[1][2] + left->m[3][2]*right->m[2][2] + left->m[3][3]*right->m[3][2]);
    result[3][3] = (left->m[3][0]*right->m[0][3] + left->m[3][1]*right->m[1][3] + left->m[3][2]*right->m[2][3] + left->m[3][3]*right->m[3][3]);
    m = &result;
}

//transforms the point p by the matrix m and put the result in q. For this function, p and q need to be diffrent variables
void matrix_xformPoint(Matrix *m, Point *p, Point *q){
    int i;
    for(i=0; i<4; i++){
        q->val[i] = m->m[i][0]*p->val[0] + m->m[i][1]*p->val[1] + m->m[i][2]*p->val[2] + m->m[i][3]*p->val[3];
    }
}

//transform the vector p by the matrix m and put the result in q. For this function, p and q need to be different variables
void matrix_xformVector(Matrix *m, Vector *p, Vector *q){
    int i;
    for(i=0; i<4; i++){
        q->val[i] = m->m[i][0]*p->val[0] + m->m[i][1]*p->val[1] + m->m[i][2]*p->val[2] + m->m[i][3]*p->val[3];
    }
}

//transform the points (and surface normals (non existant)) in the polygon p by the matrix m
void matrix_xformPolygon(Matrix *m, Polygon *p){
    int n;
    double tmp0, tmp1, tmp2;
    for(n=0; n<p->nVertex; n++){
        tmp0 = m->m[0][0]*p->vertex[n].val[0] + m->m[0][1]*p->vertex[n].val[1] + m->m[0][2]*p->vertex[n].val[2] + m->m[0][3]*p->vertex[n].val[3];
        tmp1 = m->m[1][0]*p->vertex[n].val[0] + m->m[1][1]*p->vertex[n].val[1] + m->m[1][2]*p->vertex[n].val[2] + m->m[1][3]*p->vertex[n].val[3];
        tmp2 = m->m[2][0]*p->vertex[n].val[0] + m->m[2][1]*p->vertex[n].val[1] + m->m[2][2]*p->vertex[n].val[2] + m->m[2][3]*p->vertex[n].val[3];
        p->vertex[n].val[3] = m->m[3][0]*p->vertex[n].val[0] + m->m[3][1]*p->vertex[n].val[1] + m->m[3][2]*p->vertex[n].val[2] + m->m[3][3]*p->vertex[n].val[3];
        p->vertex[n].val[0] = tmp0;
        p->vertex[n].val[1] = tmp1;
        p->vertex[n].val[2] = tmp2;
    }
}

//transform the points in the polyline p by the matrix m
void matrix_xformPolyline(Matrix *m, Polyline *p){
    int n;
    double tmp0, tmp1, tmp2;
    for(n=0; n<p->numVertex; n++){
        tmp0 = m->m[0][0]*p->vertex[n].val[0] + m->m[0][1]*p->vertex[n].val[1] + m->m[0][2]*p->vertex[n].val[2] + m->m[0][3]*p->vertex[n].val[3];
        tmp1 = m->m[1][0]*p->vertex[n].val[0] + m->m[1][1]*p->vertex[n].val[1] + m->m[1][2]*p->vertex[n].val[2] + m->m[1][3]*p->vertex[n].val[3];
        tmp2 = m->m[2][0]*p->vertex[n].val[0] + m->m[2][1]*p->vertex[n].val[1] + m->m[2][2]*p->vertex[n].val[2] + m->m[2][3]*p->vertex[n].val[3];
        p->vertex[n].val[3] = m->m[3][0]*p->vertex[n].val[0] + m->m[3][1]*p->vertex[n].val[1] + m->m[3][2]*p->vertex[n].val[2] + m->m[3][3]*p->vertex[n].val[3];
        p->vertex[n].val[0] = tmp0;
        p->vertex[n].val[1] = tmp1;
        p->vertex[n].val[2] = tmp2;
    }
}

//transform the points in line by the matrix m
void matrix_xformLine(Matrix *m, Line *line){
    double tmp0, tmp1, tmp2;
    tmp0 = m->m[0][0]*line->a.val[0] + m->m[0][1]*line->a.val[1] + m->m[0][2]*line->a.val[2] + m->m[0][3]*line->a.val[3];
    tmp1 = m->m[1][0]*line->a.val[0] + m->m[1][1]*line->a.val[1] + m->m[1][2]*line->a.val[2] + m->m[1][3]*line->a.val[3];
    tmp2 = m->m[2][0]*line->a.val[0] + m->m[2][1]*line->a.val[1] + m->m[2][2]*line->a.val[2] + m->m[2][3]*line->a.val[3];
    line->a.val[3] = m->m[3][0]*line->a.val[0] + m->m[3][1]*line->a.val[1] + m->m[3][2]*line->a.val[2] + m->m[3][3]*line->a.val[3];
    line->a.val[0] = tmp0;
    line->a.val[1] = tmp1;
    line->a.val[2] = tmp2;
    
    tmp0 = m->m[0][0]*line->b.val[0] + m->m[0][1]*line->b.val[1] + m->m[0][2]*line->b.val[2] + m->m[0][3]*line->b.val[3];
    tmp1 = m->m[1][0]*line->b.val[0] + m->m[1][1]*line->b.val[1] + m->m[1][2]*line->b.val[2] + m->m[1][3]*line->b.val[3];
    tmp2 = m->m[2][0]*line->b.val[0] + m->m[2][1]*line->b.val[1] + m->m[2][2]*line->b.val[2] + m->m[2][3]*line->b.val[3];
    line->b.val[3] = m->m[3][0]*line->b.val[0] + m->m[3][1]*line->b.val[1] + m->m[3][2]*line->b.val[2] + m->m[3][3]*line->b.val[3];
    line->b.val[0] = tmp0;
    line->b.val[1] = tmp1;
    line->b.val[2] = tmp2;
}

//premultiply the matrix by a scale matrix parameterized by sx and sy
void matrix_scale2D(Matrix *m, double sx, double sy){
    int i;
    for(i=0; i<4; i++){
        m->m[0][i] = m->m[0][i] * sx;
        m->m[1][i] = m->m[1][i] * sy;
    }
}

//premultiply the matrix by z-axis rotation matrix parameterized by cos and sin of the angle of rotation about the z-axis
void matrix_rotateZ(Matrix *m, double cth, double sth){
    int i;
    double tmp;
    for(i=0; i<4; i++){
        tmp = m->m[0][i]*cth - m->m[1][i]*sth;
        m->m[1][i] = m->m[0][i]*sth + m->m[1][i]*cth;
        m->m[0][i] = tmp;
    }
}

//premultiply the matrix by a 2D traslation matrix parameterized by tx and ty
void matrix_translate2D(Matrix *m, double tx, double ty){
    int i;
    for(i=0; i<4; i++){
        m->m[0][i] = m->m[0][i] + m->m[3][i] * tx;
        m->m[1][i] = m->m[1][i] + m->m[3][i] * ty;
    }
}

//premultiply the matrix by a 2D shear matrix parameterized by shx and shy
void matrix_shear2D(Matrix *m, double shx, double shy){
    int i;
    double tmp;
    for(i=0; i<4; i++){
        tmp = m->m[0][i] + m->m[1][i] * shx;
        m->m[1][i] = m->m[1][i] + m->m[0][i] * shy;
        m->m[0][i] = tmp;
    }
}

/*3D Matrix Functions*/

//premultiply the matrix by a translation matrix parameterized by tx, ty, and tz
void matrix_translate(Matrix *m, double tx, double ty, double tz){
    int i;
    for(i=0; i<4; i++){
        m->m[0][i] = m->m[0][i] + m->m[3][i] * tx;
        m->m[1][i] = m->m[1][i] + m->m[3][i] * ty;
        m->m[2][i] = m->m[2][i] + m->m[3][i] * tz;
    }
}

//pemultiply the matrix by a scale matrix parameterized by sx, sy, sz
void matrix_scale(Matrix *m, double sx, double sy, double sz){
    int i;
    for(i=0; i<4; i++){
        m->m[0][i] = m->m[0][i] * sx;
        m->m[1][i] = m->m[1][i] * sy;
        m->m[2][i] = m->m[2][i] * sy;
    }
}

//premultiply the matrix by a x-axis rotation matrix cos and sin about an angle of rotation about the x-axis
void matrix_rotateX(Matrix *m, double cth, double sth){
    int i;
    double tmp;
    for(i=0; i<4; i++){
        tmp = m->m[1][i]*cth - m->m[2][i]*sth;
        m->m[2][i] = m->m[1][i]*sth + m->m[2][i]*cth;
        m->m[1][i] = tmp;
    }
}

//premultiply the matrix by a y-axis rotation matrix cos and sin about an angle of rotation about the y-axis
void matrix_rotateY(Matrix *m, double cth, double sth){
    int i;
    double tmp;
    for(i=0; i<4; i++){
        tmp = m->m[0][i]*cth + m->m[2][i]*sth;
        m->m[2][i] = m->m[2][i]*cth - m->m[0][i]*2th;
        m->m[0][i] = tmp;
    }
}

//premultiply the matrix by a xyz-axis rotation matrix parameterized by the vectors u,v,w where the three vectors represent an orthonormal 3D basis
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w){
    int i;
    double tmp0, tmp1;
    for(i=0; i<4; i++){
        tmp0 = m->m[0][i]*u->val[0] + m->m[1][i]*u->val[1] + m->m[2][i]*u->val[2];
        tmp1 = m->m[0][i]*v->val[0] + m->m[1][i]*v->val[1] + m->m[2][i]*v->val[2];
        m->m[2][i] = m->m[0][i]*w->val[0] + m->m[1][i]*w->val[1] + m->m[2][i]*w->val[2];
        m->m[0][i] = tmp0;
        m->m[1][i] = tmp1;
}

//premultiply the matrix by a shear Z matrix parameterized by shx and shy
void matrix_shearZ(Matrix *m, double shx, double shy){
    int i;
    for(i=0; i<4; i++){
        m->m[0][i] = m->m[0][i] + m->m[2][i] * shx;
        m->m[1][i] = m->m[1][i] + m->m[2][i] * shy;
    }
}

//premultiply the matrix by a perspective matrix parameterized by d
void matrix_perspective(Matrix *m, double d){
    int i;
    for(i=0; i<4; i++){
        m->m[3][i] = m->m[3][i] / d;
    }
}

