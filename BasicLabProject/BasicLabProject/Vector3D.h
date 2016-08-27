#pragma once
#include<iostream>

class Vector3D {
	float x;
	float y;
	float z;
	friend std::ostream& operator<<(std::ostream &strm, const Vector3D &vector);
public:
	Vector3D() = default; 
	Vector3D(float x, float y, float z);
    Vector3D copy();
	void add(Vector3D that);
    void add(float ratio);
	float getX();
	float getY();
	float getZ();
	void set(Vector3D& that);
    void multiply(float ratio);
    void multiply(Vector3D& that);
};
/* Analog to Java.lang.Object.toString() */
std::ostream& operator<<(std::ostream &strm, const Vector3D &vector);