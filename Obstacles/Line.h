#ifndef LINE_H_
#define LINE_H_

#include <math.h>
#include "Obstacle.h"

class Line: public Obstacle
{
public:
	Line(double x1, double y1, double x2, double y2);
	virtual ~Line();
	
	void Set(double x1, double y1, double x2, double y2);

	// tavolsag ha (x1,y1) pontbol megyunk phi szog iranyaba
	// visszateres -1 ha nincs metszespont
	// negativ ha a metszespont mogottunk van
	double Distance(double x, double y, double phi);
	
	// legkisebb tavolsag (x,y) kozeppontu r sugaru koriven haladva
	// (xr,yr) pontbol, omega iranyba
	// visszateres -1 ha nincs metszespont
	double Distance(double x, double y, double r, double xr, double yr, double omega);
	
	bool Intersect(double x1, double y1, double x2, double y2);
	bool Intersect(double x, double y, double r);
	bool Intersect(Obstacle* obstacle);

	void Print();

	void getObstacle(msgobstacle* obstacle);

	double minx, miny, maxx, maxy;
protected:
	double m, b;
	bool ylonger;
};

#endif /*LINE_H_*/
