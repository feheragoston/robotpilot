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

#include "../Primitives/Net/Protocol/Protocol.h"

enum ShapeTypes {
	SHAPE_OBSTACLE,
	SHAPE_CIRCLE,
	SHAPE_LINE
};

class Obstacle {
	public:
		Obstacle();
		virtual ~Obstacle();

		/**
		 * metszes szakasszal
		 * @param x1
		 * @param y1
		 * @param x2
		 * @param y2
		 * @return
		 */
		virtual bool Intersect(double x1, double y1, double x2, double y2);

		/**
		 * metszes korrel
		 * @param x
		 * @param
		 * @param
		 * @return
		 */
		virtual bool Intersect(double x, double y, double r);

		/**
		 * metszes masik akadallyal
		 * @param obstacle
		 * @return
		 */
		virtual bool Intersect(Obstacle* obstacle);

		virtual void Print();

		int getShapeType() const { return m_shapeType; }

		virtual void getObstacle(msgobstacle* obstacle);

	protected:
		int m_shapeType;
};

#endif /* OBSTACLE_H_ */
