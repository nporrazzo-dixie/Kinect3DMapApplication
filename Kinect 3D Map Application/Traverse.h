#pragma once
class Traverse
{
public:
	Traverse(void);

	void Move_forward(double * eye, double * at, double change);
	void Move_left(double * eye, double * at, double change);
	void Move_back(double * eye, double * at, double change);
	void Move_right(double * eye,double * at, double change);
};

