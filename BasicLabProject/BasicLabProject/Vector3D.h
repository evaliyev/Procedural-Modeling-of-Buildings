#pragma once
#include<iostream>

class Vector3D {
	double x;
	double y;
	double z;
	friend std::ostream& operator<<(std::ostream &strm, const Vector3D &vector);
public:
	Vector3D() = default; 
	Vector3D(double x, double y, double z);
	void add(Vector3D that);
};
/* Analog to Java.lang.Object.toString() */
std::ostream& operator<<(std::ostream &strm, const Vector3D &vector);