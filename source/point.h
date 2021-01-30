#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;

struct Point {
	float x, y, z;

	Point(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}
	~Point() {}

	Point& setx(float a){
		x = a;
		return *this;
	}

	Point& sety(float a){
		y = a;
		return *this;
	}

	Point& operator=(Point& punto){
		x = punto.x;
		y = punto.y;
		z = punto.z;
		return *this;
	}

	void printIt() {
		cout << x << " " << y << " " << z << endl;
	}

};

#endif