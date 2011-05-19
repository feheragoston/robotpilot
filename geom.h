//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	  geom.h
//	----------
//	
//	Matematikai es koordinata-geometriai fuggvenyek 
//	prototipusai
//	
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#ifndef __GEOM_H_
#define __GEOM_H_

#include <stdio.h>
#include <math.h>


double max(double a, double b);

double min(double a, double b);

double sqr(double x);

int sgn(double x);

int powi(int a, int b);

double AngleCorrect(double angle);

double rad2deg(double angle);

double deg2rad(double angle);

double AngleDist(double angle1, double angle2);

double PointDist(double ax, double ay, double bx, double by);

double ScalarProduct(double ax, double ay, double bx, double by);

void LineLineIntersection(double x11, double y11, double x12, double y12,
			  double x21, double y21, double x22, double y22,
			  double *result_x, double *result_y);

int LineCircleIntersection(double e1x, double e1y, double e2x, double e2y,
			   double ox, double oy, double r,
			   double *result1_x, double *result1_y,
			   double *result2_x, double *result2_y);

int CircleCircleIntersection(double o1x, double o1y, double r1,
			   double o2x, double o2y, double r2,
			   double *result1_x, double *result1_y,
			   double *result2_x, double *result2_y);

void PointProjectToLine(double px, double py, 
			double e1x, double e1y,
			double e2x, double e2y,
			double *result_x, double *result_y);

void PointProjectToCircle(double px, double py, 
			  double ox, double oy, double r,
			  double *result_x, double *result_y);

void PointTurn(double px, double py, 
	       double ox, double oy, double angle,
	       double *result_x, double *result_y);

double M22Det(double M[2][2]);
double M33Det(double M[3][3]);
int M33Inv(double M[3][3], double Minv[3][3]);
void M33Mult(double M1[3][3], double M2[3][3], double Mr[3][3]);
void M33V3Mult(double M[3][3], double V[3], double Vr[3]);

void TM33Build(double tx, double ty, double alpha, double T[3][3]);
void TM33Extract(double T[3][3], double* tx, double* ty, double* alpha);
void TM33Inv(double T[3][3], double Tinv[3][3]);

void printM33(double M[3][3]);

#endif //__GEOM_H_

