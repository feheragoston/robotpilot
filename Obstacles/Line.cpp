#include "Line.h"

Line::Line(double x1, double y1, double x2, double y2) {
	this->Set(x1, y1, x2, y2);
}

void Line::Set(double x1, double y1, double x2, double y2) {
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
}

double Line::Distance(double x, double y, double phi) {
	phi = AngleCorrect(phi);
	float m2; // metszo egyenes
	float b2; // egyenletenek parameterei
	float rx; // metszespont x
	float ry; // metszespont y

	if ((phi > -M_PI/4 && phi < M_PI/4) || phi < -3 * M_PI/4 || phi > 3 * M_PI/4) {
		m2 = sin(phi) / cos(phi);
		b2 = y - m2 * x;
		if (ylonger) {
			ry = (m2 * b + b2) / (1 - m2 * m);
			rx = m * ry + b;
		} else {
			if (fabs(m2 - m) < EPSILON) {
				// parhuzamosak
				return -1;
			}
			rx = (b2 - b) / (m - m2);
			ry = m * rx + b;
		}
	} else {
		m2 = cos(phi) / sin(phi);
		b2 = x - m2 * y;
		if (ylonger) {
			if (fabs(m2 - m) < EPSILON) {
				// parhuzamosak
				return -1;
			}
			ry = (b2 - b) / (m - m2);
			rx = m * ry + b;
		} else {
			rx = (m2 * b + b2) / (1 - m2 * m);
			ry = m * rx + b;
		}
	}
	if (rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
		if (fabs(AngleDist(atan2(ry - y, rx - x), phi)) < M_PI / 2) {
			return sqrt(sqr(x - rx) + sqr(y - ry));
		} else {
			return -sqrt(sqr(x - rx) + sqr(y - ry));
		}
	}
	return -1;
}

double Line::Distance(double x, double y, double r, double xr, double yr,
		double omega) {
	double dist;
	double disttemp;
	double A;
	double B;
	double C;
	double D;
	
	dist = 2 * M_PI; // maximalis tavolsag ezen a koriven

	if (ylonger) {
		A = sqr(m) + 1;
		B = 2 * (m * (b - x) - y);
		C = sqr(b - x) + sqr(y) - sqr(r);
		D = sqr(B) - 4 * A * C;

		if (D > EPSILON) {
			// ket megoldas
			double ry = (-B + sqrt(D)) / (2 * A);
			double rx = m * ry + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}

			ry = (-B - sqrt(D)) / (2 * A);
			rx = m * ry + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}
		} else if (fabs(D) < EPSILON) {
			// egy megoldas
			double ry = -B / (2 * A);
			double rx = m * ry + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}
		} else {
			// nincs megoldas
			return -1;
		}
	} else {
		A = sqr(m) + 1;
		B = 2 * (m * (b - y) - x);
		C = sqr(b - y) + sqr(x) - sqr(r);
		D = sqr(B) - 4 * A * C;

		if (D > EPSILON) {
			// ket megoldas
			double rx = (-B + sqrt(D)) / (2 * A);
			double ry = m * rx + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}

			rx = (-B - sqrt(D)) / (2 * A);
			ry = m * rx + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}

		} else if (fabs(D) < EPSILON) {
			// egy megoldas
			double rx = -B / (2 * A);
			double ry = m * rx + b;
			if (omega > 0) {
				disttemp = atan2(ry - y, rx - x) - atan2(yr - y, xr - x);
			} else {
				disttemp = atan2(yr - y, xr - x) - atan2(ry - y, rx - x);
			}
			while (disttemp < 0)
				disttemp += 2 * M_PI;
			if (disttemp < dist && rx >= minx && rx <= maxx && ry >= miny && ry <= maxy) {
				dist = disttemp;
			}
		} else {
			// nincs megoldas
			return -1;
		}
	}
	return dist * r;
}

bool Line::Intersect(double x1, double y1, double x2, double y2) {
	double phi = atan2(y2 - y1, x2 - x1);
	double d = sqrt(sqr(x2 - x1) + sqr(y2 - y1));
	double d2 = Distance(x1, y1, phi);
	return d > d2 && d2 > 0;
}

Line::~Line() {
}
