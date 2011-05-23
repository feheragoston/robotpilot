/*
 * Circle.cpp
 *
 *  Created on: 2010.04.30.
 *      Author: Agoston
 */

#include "Circle.h"

Circle::Circle(double x, double y, double r) {
	this->Set(x, y, r);
}

void Circle::Set(double x, double y, double r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

bool Circle::Intersect(double x1, double y1, double x2, double y2) {
	double m, b;
	double minx, miny, maxx, maxy;
	bool ylonger;
	double A, B, C, D;

	double r2 = sqr(r);
	if (sqr(x1 - x) + sqr(y1 - y) < r2) {
		return true;
	}
	if (sqr(x2 - x) + sqr(y2 - y) < r2) {
		return true;
	}

	if (fabs(y2 - y1) > fabs(x2 - x1)) {
		m = (x2 - x1) / (y2 - y1);
		b = x1 - m * y1;
		ylonger = true;
	} else {
		m = (y2 - y1) / (x2 - x1);
		b = y1 - m * x1;
		ylonger = false;
	}

	if (x1 < x2) {
		minx = x1 - EPSILON;
		maxx = x2 + EPSILON;
	} else {
		minx = x2 - EPSILON;
		maxx = x1 + EPSILON;
	}
	if (y1 < y2) {
		miny = y1 - EPSILON;
		maxy = y2 + EPSILON;
	} else {
		miny = y2 - EPSILON;
		maxy = y1 + EPSILON;
	}

	if (ylonger) {
		A = sqr(m) + 1;
		B = 2 * (m * (b - x) - y);
		C = sqr(b - x) + sqr(y) - r2;
		D = sqr(B) - 4 * A * C;
	} else {
		A = sqr(m) + 1;
		B = 2 * (m * (b - y) - x);
		C = sqr(b - y) + sqr(x) - r2;
		D = sqr(B) - 4 * A * C;
	}

	if (ylonger) {
		if (D > EPSILON) {
			// ket megoldas
			double ry = (-B + sqrt(D)) / (2 * A);
			double rx = m * ry + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
			ry = (-B - sqrt(D)) / (2 * A);
			rx = m * ry + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
		} else if (fabs(D) < EPSILON) {
			// egy megoldas
			double ry = -B / (2 * A);
			double rx = m * ry + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
		} else {
			// nincs megoldas
			return false;
		}
	} else {
		if (D > EPSILON) {
			// ket megoldas
			double rx = (-B + sqrt(D)) / (2 * A);
			double ry = m * rx + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
			rx = (-B - sqrt(D)) / (2 * A);
			ry = m * rx + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
		} else if (fabs(D) < EPSILON) {
			// egy megoldas
			double rx = -B / (2 * A);
			double ry = m * rx + b;
			if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				return true;
			}
		} else {
			// nincs megoldas
			return false;
		}
	}
	return false;
}

Circle::~Circle() {
}
