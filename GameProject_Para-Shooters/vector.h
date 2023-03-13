#pragma once
#include <cmath>
#include <utility>
#define PI acos(-1)
using namespace std;

//2D Vector
class Dir_Vector {
public:
	double x, y;
	Dir_Vector();
	Dir_Vector(double x, double y);
	Dir_Vector(pair<int, int>p);
	void setpoint(double x, double y);
	void rotate(double rotate);
	Dir_Vector operator +(const Dir_Vector &tmp) const;
	Dir_Vector operator -(const Dir_Vector &tmp) const;
	Dir_Vector operator *(const double &num) const;
	Dir_Vector operator /(const double &num) const;
	bool operator ==(const Dir_Vector& tmp) const;

	double CalcSize() { return sqrt(x * x + y * y); }
	static double Cross(const Dir_Vector& v1, const Dir_Vector& v2) { return v1.x * v2.y - v1.y * v2.x; }
	Dir_Vector Normalize() { double size = this->CalcSize(); return Dir_Vector(x, y) / size; }
	static bool CheckHit(const Dir_Vector& s1, const Dir_Vector& e1, const Dir_Vector& s2, const Dir_Vector& e2);
	//typical vectors
	static const Dir_Vector Zero;
	static const Dir_Vector Front;
	static const Dir_Vector Back;
	static const Dir_Vector Up;
	static const Dir_Vector Down;
};