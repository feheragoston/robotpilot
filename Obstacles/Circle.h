/*
 * Circle.h
 *
 *  Created on: 2010.04.30.
 *      Author: Agoston
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Obstacle.h"

class Circle: public Obstacle {
	public:
		Circle(double x, double y, double r);
		virtual ~Circle();

		void Set(double x, double y, double r);

		bool Intersect(double x1, double y1, double x2, double y2);

	protected:
		double x, y, r;
};

#endif /* CIRCLE_H_ */
