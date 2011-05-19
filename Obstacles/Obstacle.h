/*
 * Obstacle.h
 *
 *  Created on: 2010.04.30.
 *      Author: Agoston
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "../misc.h"
#include "../geom.h"

class Obstacle {
	public:
		Obstacle();
		virtual ~Obstacle();

		virtual bool Intersect(double x1, double y1, double x2, double y2);
};

#endif /* OBSTACLE_H_ */
