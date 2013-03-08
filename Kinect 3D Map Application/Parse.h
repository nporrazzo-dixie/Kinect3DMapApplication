#pragma once
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Point{
	float X;
	float Y;
	float Z;
	double R;
	double G;
	double B;
};

class Parse
{
public:
	Parse();
	Parse(string filename);
	~Parse(void);

	bool Read();

private:
	Point Create_point(string line);
public:
	void Add_point(Point p);
	Point Get_point(int i);
	vector<Point> Get_points();

private:
	string m_filename;
	vector<Point> m_points;
};

