#include "Vector3D.h"


Vector3D::Vector3D(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
};
 
void Vector3D::add(Vector3D that) {
	this->x = that.x;
	this->y = that.y;
	this->z = that.z;
}
float Vector3D::getX() { return this->x; }

float Vector3D::getY() { return this->y; }

float Vector3D::getZ() { return this->z; }

std::ostream& operator<<(std::ostream &strm, const Vector3D &vector) {
	return strm << "V("<<vector.x<<","<< vector.y <<","<< vector.z <<")";
}
