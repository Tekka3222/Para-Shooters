#include "vector.h"
#include <iostream>

using namespace std;

const Dir_Vector Dir_Vector::Zero(0.0, 0.0);
const Dir_Vector Dir_Vector::Front(1.0, 0.0);
const Dir_Vector Dir_Vector::Back(-1.0, 0.0);
const Dir_Vector Dir_Vector::Up(0.0, -1.0);
const Dir_Vector Dir_Vector::Down(0.0, 1.0);

Dir_Vector::Dir_Vector()
	:x(0.0)
	,y(0.0)
{}

Dir_Vector::Dir_Vector(double x, double y)
	: x(x)
	, y(y)
{}

Dir_Vector::Dir_Vector(pair<int, int>p)
	: x(p.first)
	, y(p.second)
{}

void Dir_Vector::setpoint(double x, double y) {
	this->x = x;
	this->y = y;
}

void Dir_Vector::rotate(double rotate) {
	double rad = rotate * PI / 180;
	double tmpx = x * cos(rad) - y * sin(rad);
	y = x * sin(rad) + y * cos(rad);
	x = tmpx;
}

Dir_Vector Dir_Vector::operator +(const Dir_Vector &tmp) const {
	return Dir_Vector(x + tmp.x, y + tmp.y);
}

Dir_Vector Dir_Vector::operator -(const Dir_Vector &tmp) const {
	return Dir_Vector(x - tmp.x, y - tmp.y);
}

Dir_Vector Dir_Vector::operator *(const double& num) const {
	return Dir_Vector(x * num, y * num);
}

Dir_Vector Dir_Vector::operator /(const double& num) const {
	return Dir_Vector(x / num, y / num);
}

bool Dir_Vector::operator ==(const Dir_Vector& tmp) const {
	return (x == tmp.x && y == tmp.y);
}

bool Dir_Vector::CheckHit(const Dir_Vector& s1, const Dir_Vector& e1, const Dir_Vector& s2, const Dir_Vector& e2) {
	double c1 = Cross((s1 - s2), (e2 - s2).Normalize());
	double c2 = Cross((e1 - s2), (e2 - s2).Normalize());
	if (c1 * c2 <= 0) {
		c1 = abs(c1), c2 = abs(c2);
		Dir_Vector p = s1 + (e1 - s1) * (c1 / (c1 + c2));
		double d = (e2 - s2).CalcSize();
		if ((s2 - p).CalcSize() <= d && (e2 - p).CalcSize() <= d) {
			return true;
		}
	}
	return false;
}

