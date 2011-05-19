//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	  geom.c
//	----------
//	
//	Kulonbozo matematikai es koordinata-geometriai fuggvenyek 
//	
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "geom.h"


//////////////////////////////////////////////////////////////////
// 
// 	Maximum

double max(double a, double b)
{
	if (a < b)
		return b;
	return a;
}

//////////////////////////////////////////////////////////////////
// 
// 	Minimum

double min(double a, double b)
{
	if (a > b)
		return b;
	return a;
}

//////////////////////////////////////////////////////////////////
// 
// 	Negyzetkepzes

double sqr(double x)
{
	return (double)(x*x);
}

//////////////////////////////////////////////////////////////////
// 
// 	Elojelkepzes

int sgn(double x)
{
	if(x > 0) return 1;
	else	if(x == 0) return 0;
		else return -1;
}

//////////////////////////////////////////////////////////////////
// 
// 	Hatvanyozas a^b

int powi(int a, int b) {
	int ret = 1;
	for (;b > 0; b--) {
		ret *= a;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////
// 
// 	Szog lekepezese a [+pi,-pi) tartomanyra

double AngleCorrect(double angle)
{
	while(angle <= -M_PI) angle += 2*M_PI;
	while(angle > M_PI) angle -= 2*M_PI;
	return angle;
}

//////////////////////////////////////////////////////////////////
// 
// 	Valtas fok es radian kozott

double rad2deg(double angle)
{
	return angle*180/M_PI;
}


double deg2rad(double angle)
{
	return (angle*M_PI/180);
}

//////////////////////////////////////////////////////////////////
// 
// 	Forgasszogek kulonbsege
// 
// 	angle1 es angle2 kulonbseget adja vissza, 
// 	lekepezve a [+pi,-pi) tartomanyra

double AngleDist(double angle1, double angle2)
{	
	double ang1, ang2, diff;

	ang1 = AngleCorrect(angle1);
	ang2 = AngleCorrect(angle2);
	diff = AngleCorrect(ang1-ang2);
	return diff;
}

//////////////////////////////////////////////////////////////////
// 
// 	Ket pont euklideszi tavolsaganak kiszamitasa
// 
// 	Parameterek:	
// 		ax, ay: egyik pont
// 		bx, by: masik pont

double PointDist(double ax, double ay, double bx, double by)
{
	return sqrt(sqr(bx-ax)+sqr(by-ay));
}

//////////////////////////////////////////////////////////////////
// 
// 	Ket vektor skalarszorzata
// 	
// 	Parameterek:	
// 		ax, ay: egyik vektor
// 		bx, by: masik vektor

double ScalarProduct(double ax, double ay, double bx, double by)
{
	return ax*bx+ay*by;
}

//////////////////////////////////////////////////////////////////
// 
// 	Ket egyenes metszespontjanak kiszamitasa 
// 	
// 	Mindket egyenes ket ponttal van definialva.
// 	Parameterek:	
// 		x11, y11: az 1. egyenes 1. pontja
// 		x12, y12: az 1. egyenes 2. pontja
// 		x21, y21: a 2. egyenes 1. pontja
// 		x22, y22: a 2. egyenes 2. pontja
// 		result_x, result_y: a metszespont

void LineLineIntersection(double x11, double y11, double x12, double y12,
			  double x21, double y21, double x22, double y22,
			  double *result_x, double *result_y)
{
	double A, B, C, D, E, F;

	A = y12-y11;
	B = x12-x11;
	C = A*x11-B*y11;
	D = y22-y21;
	E = x22-x21;
	F = D*x21-E*y21;

	if(A!=0)
	{
		*result_y = (F - D*C/A) / (B*D/A - E);
		*result_x = C/A + (B/A)*(*result_y);
	}
	else
	{
		*result_x = (F - E*C/B) / (D - A*E/B);
		*result_y = -C/B + (A/B)*(*result_x);
	}
}

//////////////////////////////////////////////////////////////////
// 
// 	Egy egyenes es egy kor metszespontjainak meghatarozasa
// 	
// 	Parameterek:	
// 		e1x, e1y:	az egyenes 1. pontja
// 		e2x, e2y:	az egyenes 2. pontja
// 		ox, oy:		a kor kozeppontja
// 		r: 		a kor sugara
// 		result1_x, result1_y: az egyik metszespont
// 		result2_x, result2_y: a masik metszespont
// 			
// 	Visszateresi erteke:
// 		0: nincs metszespont
// 		1: erintopont
// 		2: ket metszespont

int LineCircleIntersection(double e1x, double e1y, double e2x, double e2y,
			   double ox, double oy, double r,
			   double *result1_x, double *result1_y,
			   double *result2_x, double *result2_y)
{
	double v1, v2;	// az egyenes iranyvektoranak koordinatai
	double k, A, B, C;

	v1 = e2x - e1x;
	v2 = e2y - e1y;
	k = v2*e1x - v1*e1y;
	if(v2!=0)
	{
		A = sqr(v1/v2)+1;
		B = 2*(k/v2-ox)*(v1/v2) - 2*oy;
		C = sqr(k/v2-ox) + sqr(oy) - sqr(r);

		if((sqr(B)-4*A*C) < 0) return 0;	// nincs megoldas
		else if((sqr(B)-4*A*C) == 0)		// 1 megoldas van
		{
			*result1_y = *result2_y = -B / (2*A);
			*result1_x = *result2_x = (k + v1*(*result1_y)) / v2;
			return 1;
		}
		else					// 2 megoldas van
		{
			*result1_y = (-B + sqrt(sqr(B)-4*A*C)) / (2*A);
			*result2_y = (-B - sqrt(sqr(B)-4*A*C)) / (2*A);
			*result1_x = (k + v1*(*result1_y)) / v2;
			*result2_x = (k + v1*(*result2_y)) / v2;
			return 2;
		}
	}
	else
	{
		A = sqr(v2/v1)+1;
		B = -2*(k/v1+oy)*(v2/v1) - 2*ox;
		C = sqr(k/v1+oy) + sqr(ox) - sqr(r);

		if((sqr(B)-4*A*C) < 0) return 0;	// nincs megoldas
		else if((sqr(B)-4*A*C) == 0)		// 1 megoldas van
		{
			*result1_x = *result2_x = -B / (2*A);
			*result1_y = *result2_y = (k - v2*(*result1_x)) / (-v1);
			return 1;
		}
		else					// 2 megoldas van
		{
			*result1_x = (-B + sqrt(sqr(B)-4*A*C)) / (2*A);
			*result2_x = (-B - sqrt(sqr(B)-4*A*C)) / (2*A);
			*result1_y = (k - v2*(*result1_x)) / (-v1);
			*result2_y = (k - v2*(*result2_x)) / (-v1);
			return 2;
		}

	}
	
}

//////////////////////////////////////////////////////////////////
// 
// 	Ket kor metszespontjainak meghatarozasa
// 	
// 	Parameterek:
// 		o1x, o1y:	az egyik kor kozeppontja
// 		r1:		az egyik kor sugara
// 		o2x, o2y:	a masik kor kozeppontja
// 		r2:		a masik kor sugara
// 		result1_x, result1_y: az egyik metszespont
// 		result2_x, result2_y: a masik metszespont
//
// 	Visszateresi erteke:
// 		0: nincs metszespont
// 		1: erintopont
// 		2: ket metszespont

int CircleCircleIntersection(double o1x, double o1y, double r1,
			   double o2x, double o2y, double r2,
			   double *result1_x, double *result1_y,
			   double *result2_x, double *result2_y)

{
	double dx, dy, d, alpha;
	double T[3][3], Tinv[3][3];
	double o1[3], o2[3], o1_v[3], o2_v[3];
	double res1_v[3], res2_v[3], res1[3], res2[3];
	double a, D;
	int rescnt = 0;

// Eloszor a pontokat attranszformaljuk egy masik koordnata-rendszerbe,
// ahol az egyik kozeppont (0,0) lesz, a masik (0,d)
	dx = o2x-o1x;
	dy = o2y-o1y;
	d = sqrt(dx*dx + dy*dy);
	printf("d = %f\n", d);

	alpha = atan2(dy, dx);
	printf("alpha = %f\n", alpha);

	TM33Build(o1x, o1y, alpha, T);
	TM33Inv(T, Tinv);

	o1[0] = o1x;
	o1[1] = o1y;
	o1[2] = 1;

	o2[0] = o2x;
	o2[1] = o2y;
	o2[2] = 1;

	M33V3Mult(Tinv, o1, o1_v);
	M33V3Mult(Tinv, o2, o2_v);

/*	printf("o1  : %f\t%f\t%f\n", o1[0], o1[1], o1[2]);
	printf("o1_v: %f\t%f\t%f\n", o1_v[0], o1_v[1], o1_v[2]);
	printf("o2  : %f\t%f\t%f\n", o2[0], o2[1], o2[2]);
	printf("o2_v: %f\t%f\t%f\n", o2_v[0], o2_v[1], o2_v[2]);
*/
// Kiszamoljuk a metszespontok x es y koordinatait
	res1_v[0] = (d*d - r2*r2 + r1*r1) / (2*d);
	res2_v[0] = res1_v[0];

	D = (-d+r2-r1)*(-d-r2+r1)*(-d+r2+r1)*(d+r2+r1);

	if(D < 0) {
		res1_v[1] = 0;
		res2_v[1] = 0;
		rescnt = 0;
	}
	else if(D == 0) {
		res1_v[1] = 0;
		res2_v[1] = 0;
		rescnt = 1;
	}
	else {
		a = 1/d * sqrt(D);
		res1_v[1] = a/2;
		res2_v[1] = -a/2;
		rescnt = 2;
	}
	res1_v[2] = 1;
	res2_v[2] = 1;
	
// Az eredmenyt visszatranszformaljuk az eredeti koordinatarendszerbe
	M33V3Mult(T, res1_v, res1);
	M33V3Mult(T, res2_v, res2);

/*	printf("res1_v: %f\t%f\t%f\n", res1_v[0], res1_v[1], res1_v[2]);
	printf("res1  : %f\t%f\t%f\n", res1[0], res1[1], res1[2]);
	printf("res2_v: %f\t%f\t%f\n", res2_v[0], res2_v[1], res2_v[2]);
	printf("res2  : %f\t%f\t%f\n", res2[0], res2[1], res2[2]);
*/
	*result1_x = res1[0];
	*result1_y = res1[1];
	*result2_x = res2[0];
	*result2_y = res2[1];

	return rescnt;

}

//////////////////////////////////////////////////////////////////
// 
// 	Pontot vetitese egy ket ponttal definialt egyenesre
// 
// 	Parameterek:
// 		px, py:		a vetitendo pont
// 		e1x, e1y:	az egyenes 1. pontja
// 		e2x, e2y:	az egyenes 2. pontja
// 		result_x, result_y: a vetulet

void PointProjectToLine(double px, double py, 
			double e1x, double e1y,
			double e2x, double e2y,
			double *result_x, double *result_y)
{
	double scalartmp;

	scalartmp = ScalarProduct((px-e1x),(py-e1y),(e2x-e1x),(e2y-e1y)) / PointDist(e2x, e2y, e1x, e1y);

	*result_x = e1x + scalartmp * (e2x-e1x) / PointDist(e2x, e2y, e1x, e1y);
	*result_y = e1y + scalartmp * (e2y-e1y) / PointDist(e2x, e2y, e1x, e1y);
}

//////////////////////////////////////////////////////////////////
// 
// 	Pont vetitese korre
// 	
// 	Parameterek:
// 		px, py:	a vetitendo pont
// 		ox, oy:	a kor kozeppontja
// 		r:	a kor sugara
// 		result_x, result_y: a vetulet

void PointProjectToCircle(double px, double py, 
			  double ox, double oy, double r,
			  double *result_x, double *result_y)
{
	double d;
	
	d = PointDist(px, py, ox, oy);
	
	if(d!=0)
	{	
		*result_x = ox + r * (px-ox) / d;
		*result_y = oy + r * (py-oy) / d;
	}
	else
	{
		*result_x = ox - r;
		*result_y = oy;

	}
}

//////////////////////////////////////////////////////////////////
// 
// 	Pontot elforgatasa egy masik pont korul
// 
// 	Parameterek:
// 		px, py:	forgatando pont
// 		ox, oy:	forgatasi kozeppont
// 		angle:	forgatasi szog (rad)
// 		result_x, result_y: az elforgatott pont

void PointTurn(double px, double py, 
	       double ox, double oy, double angle,
	       double *result_x, double *result_y)
{
	*result_x = ox + (px-ox)*cos(angle) - (py-oy)*sin(angle);
	*result_y = oy + (px-ox)*sin(angle) + (py-oy)*cos(angle);
}

//////////////////////////////////////////////////////////////////
//
// 	2x2-es matrix determinansa

double M22Det(double M[2][2]) 
{
	return M[0][0]*M[1][1] - M[0][1]*M[1][0];
}

//////////////////////////////////////////////////////////////////
//
// 	3x3-as matrix determinansa

double M33Det(double M[3][3]) 
{
	double a,b,c,d,e,f,g,h,i;

	a = M[0][0];
	b = M[0][1];
	c = M[0][2];
	d = M[1][0];
	e = M[1][1];
	f = M[1][2];
	g = M[2][0];
	h = M[2][1];
	i = M[2][2];
	
	return a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g);
}

//////////////////////////////////////////////////////////////////
// 
// 	3x3-as matrix invertalasa
// 
// 	Parameterek: 
// 		M:	kiindulasi matrix
// 		Minv:	invertalt matrix
// 	
// 	Visszateresi erteke:
// 		Sikeres invertalas: 0
// 		A matrix nem invertalhato: -1

int M33Inv(double M[3][3], double Minv[3][3]) 
{

	double SubM[3][3][2][2];
	double C[3][3];
	double detM;
	int i,j,k,l,m,n;

/*	printf("M =\n");
	for(i=0;i<3;i++)
		printf("%7.2lf\t%7.2lf\t%7.2lf\n", M[i][0],M[i][1],M[i][2]);
	printf("\n");
*/
	detM = M33Det(M);
	//printf("detM = %lf\n", detM);

	if(detM == 0.0) {
		printf("Error: Matrix is not invertible.\n");
		return -1;
	}

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			//printf("SubM(%d,%d):\n",i,j);
			m = 0;
			n = 0;
			for(k=0;k<3;k++) {
				for(l=0;l<3;l++) {
					if(k!=i && l!=j) {
						SubM[i][j][m][n] = M[k][l];
						//printf("%7.2lf(%d,%d)\t", SubM[i][j][m][n], m,n);
					}
					if(l!=j) n++;
				}
				if(k!=i) m++;
				n = 0;
				//if(k!=i) printf("\n");
			}
			C[i][j] = pow(-1.0, i+j) * M22Det(SubM[i][j]);
			//printf("C(%d,%d) = %7.2lf\n", i, j, C[i][j]);
			//printf("\n");	
		}
	}
	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			Minv[i][j] = 1/detM * C[j][i];
			//printf("Minv(%d,%d) = %f\n",i,j,Minv[i][j]);
		}
	}
/*	printf("Minv =\n");
	for(i=0;i<3;i++)
		printf("%7.2lf\t%7.2lf\t%7.2lf\n", Minv[i][0],Minv[i][1],Minv[i][2]);
	printf("\n");
*/	return 0;
}

//////////////////////////////////////////////////////////////////
// 
// 	3x3-as matrixok szorzasa
// 
// 	Parameterek: 
// 		M1, M2:	osszeszorzando matrixok
// 		Mr:	a szorzat (M1*M2)
// 	

void M33Mult(double M1[3][3], double M2[3][3], double Mr[3][3]) {
	int i,j;

	for(i=0;i<3;i++) {
		for(j=0;j<3;j++) {
			Mr[i][j] = M1[i][0]*M2[0][j] + M1[i][1]*M2[1][j] + M1[i][2]*M2[2][j];
		}
	}
/*	printf("Mr =\n");
	for(i=0;i<3;i++)
		printf("%7.2lf\t%7.2lf\t%7.2lf\n", Mr[i][0],Mr[i][1],Mr[i][2]);
	printf("\n");
*/
}

//////////////////////////////////////////////////////////////////
// 
// 	3x3-as matrix es 3 elemu oszlopvektor szorzasa
// 
// 	Parameterek: 
// 		M:	matrix
// 		V:	oszlopvektor
// 		Vr:	a szorzat (M*V)

void M33V3Mult(double M[3][3], double V[3], double Vr[3]) {
	int i;

	for(i=0;i<3;i++) {
		Vr[i] = M[i][0]*V[0] + M[i][1]*V[1] + M[i][2]*V[2];
	}
/*	printf("Vr =\n");
	for(i=0;i<3;i++)
		printf("%7.2lf\n", Vr[i]);
	printf("\n");
*/
}

//////////////////////////////////////////////////////////////////
// 
// 	Ket dimenzios homogen transzformacios matrix felepitese
// 	a transzformacios adatokbol
// 	
// 	Parameterek:
// 		tx, ty:	eltolas merteke
// 		alpha:	elforgatas szoge
// 		T:	a transzformacios matrix

void TM33Build(double tx, double ty, double alpha, double T[3][3]) {

	T[0][0] = cos(alpha);
	T[0][1] = -sin(alpha);
	T[0][2] = tx;
	T[1][0] = sin(alpha);
	T[1][1] = cos(alpha);
	T[1][2] = ty;
	T[2][0] = 0.0;
	T[2][1] = 0.0;
	T[2][2] = 1.0;
}

//////////////////////////////////////////////////////////////////
// 
// 	Transzformacios adatok kinyerese ket dimenzios 
// 	homogen transzformacios matrixbol
// 	
// 	Parameterek:
// 		T:	a transzformacios matrix
// 		tx, ty:	eltolas merteke
// 		alpha:	elforgatas szoge

void TM33Extract(double T[3][3], double* tx, double* ty, double* alpha) {

	*tx = T[0][2];
	*ty = T[1][2];
	*alpha = atan2(T[1][0],T[0][0]);
}

//////////////////////////////////////////////////////////////////
// 
// 	Homogen transzformacios matrix invertalasa
// 	
// 	Parameterek:
// 		T:	transzformacios matrix
// 		Tinv:	inverz transzformacios matrix

void TM33Inv(double T[3][3], double Tinv[3][3]) {

	Tinv[0][0] = T[0][0];
	Tinv[0][1] = T[1][0];
	Tinv[0][2] = -T[0][0]*T[0][2]-T[1][0]*T[1][2];
	Tinv[1][0] = T[0][1];
	Tinv[1][1] = T[1][1];
	Tinv[1][2] = T[1][0]*T[0][2]-T[1][1]*T[1][2];
	Tinv[2][0] = T[2][0];
	Tinv[2][1] = T[2][1];
	Tinv[2][2] = T[2][2];
}

//////////////////////////////////////////////////////////////////
// 
//	3x3-as matrix elemeinek kiiratasa

void printM33(double M[3][3]) {

	int i;

	for(i=0;i<3;i++)
		printf("%7.2g\t%7.2g\t%7.2g\n", M[i][0],M[i][1],M[i][2]);
	printf("\n");
}

//////////////////////////////////////////////////////////////////



