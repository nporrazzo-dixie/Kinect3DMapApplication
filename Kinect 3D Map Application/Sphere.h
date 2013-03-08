#pragma once
#include <cmath>
#include "Parse.h"

class Sphere
{
public:
	Sphere(Point point);
	~Sphere(void);

	void Update(double delta_time, double current_time);
	void Draw();

private:
	Point m_point;
};

