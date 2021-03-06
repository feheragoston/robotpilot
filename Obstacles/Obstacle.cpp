/*
 * Obstacle.cpp
 *
 *  Created on: 2010.04.30.
 *      Author: Agoston
 */

#include "Obstacle.h"

Obstacle::Obstacle() {
	m_shapeType = SHAPE_OBSTACLE;
}

Obstacle::~Obstacle() {
}

bool Obstacle::Intersect(double x1, double y1, double x2, double y2) {
	return false;
}

bool Obstacle::Intersect(double x, double y, double r) {
	return false;
}

bool Obstacle::Intersect(Obstacle* obstacle) {
	return false;
}

void Obstacle::Print() {
	printf("Obstacle\n");
}

void Obstacle::getObstacle(msgobstacle* obstacle) {
	obstacle->type = m_shapeType;
	for (int i = 0; i < 4; i++) {
		obstacle->d[i] = 0;
	}
}
