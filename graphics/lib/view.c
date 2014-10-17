/* view function declarations
 Chris Burnham and Victoria Tisdale 10/16
*/

#include "view.h"

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
	double b;

	matrix_identity(vtm);
	matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
	printf("After VRP\n");	
	matrix_print(vtm, stdout);

	vpn = view->vpn;
	vector_cross(&view->vup, &vpn, &u);
	vector_cross(&vpn, &u, &vup);
	vector_normalize(&u);
	vector_normalize(&vup);
	vector_normalize(&vpn);

	matrix_rotateXYZ(vtm, &u, &vup, &vpn);
	printf("After rotateXYZ\n");
	matrix_print(vtm, stdout);
	matrix_translate(vtm, 0.0, 0.0, view->d);
	printf("After COP\n");
	matrix_print(vtm, stdout);
	
	// in lecture notes here (6 and 7) it says to shear but as we only have d to define the COP I don't think we have to

	b = view->d + view->b;
	
	matrix_scale(vtm, ((2.0*view->d) / (b*view->du)), ((2.0*view->d) / (b*view->dv)), (1/b));
	printf("After scale CVV\n");
	matrix_print(vtm, stdout);
	
	matrix_perspective(vtm, view->d);
	printf("After perspective\n");
	matrix_scale2D(vtm, (-view->screenx / (2.0*view->d)), (-view->screeny / (2.0*view->d)));
	matrix_translate2D(vtm, (view->screenx / 2.0), (view->screenx / 2.0));
	printf("final\n");
	matrix_print(vtm, stdout);
}
