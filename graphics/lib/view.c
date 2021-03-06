/* view function declarations
 Chris Burnham and Victoria Tisdale 10/16
*/

#include "view.h"

//sets up the view
void view2D_set(View2D *view, Point *vrp, double dx, Vector *x, double screenx, double screeny){
	point_copy(&(view->vrp), vrp);
	view->dx = dx;
	vector_copy(&(view->x), x);
	view->screenx = screenx;
	view->screeny = screeny;
}

//sets the vtm to be the virw transfermation defined by the 2D View structure
void matrix_setView2D(Matrix *vtm, View2D *view){
	matrix_identity(vtm);
	matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
	matrix_rotateZ(vtm, view->x.val[0], -view->x.val[1]);
	matrix_scale2D(vtm, (view->screenx / view->dx), (-view->screenx / view->dx)); // S(C/ du, R/ dv) where dv = du*R/ C
	matrix_translate2D(vtm, (view->screenx / 2.0), (view->screeny / 2.0));
	
}

//3D viewing pipeline. VTM is complete view matrix. none of the values of the View structure should be edited.
void matrix_setView3D(Matrix *vtm, View3D *view){
	Vector u, vup, vpn;
	double b, d;

	matrix_identity(vtm);
	matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);

	vpn = view->vpn;
	vector_cross(&view->vup, &vpn, &u);
	vector_cross(&vpn, &u, &vup);
	vector_normalize(&u);
	vector_normalize(&vup);
	vector_normalize(&vpn);

	matrix_rotateXYZ(vtm, &u, &vup, &vpn);
	matrix_translate(vtm, 0.0, 0.0, view->d);
	
	// in lecture notes here (6 and 7) it says to shear but as we only have d to define the COP I don't think we have to

	b = view->d + view->b;
	
	matrix_scale(vtm, ((2.0*view->d) / (b*view->du)), ((2.0*view->d) / (b*view->dv)), (1/b));
	
	d = view->d / b;
	matrix_perspective(vtm, d);
	matrix_scale2D(vtm, (-view->screenx / (2.0*d)), (-view->screeny / (2.0*d)));
	matrix_translate2D(vtm, (view->screenx / 2.0), (view->screeny / 2.0));
}
