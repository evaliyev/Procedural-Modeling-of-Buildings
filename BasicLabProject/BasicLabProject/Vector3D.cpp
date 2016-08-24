#include "Vector3D.h"


Vector3D::Vector3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
};
 
void Vector3D::add(Vector3D that) {
	this->x = that.x;
	this->y = that.y;
	this->z = that.z;
};

std::ostream& operator<<(std::ostream &strm, const Vector3D &vector) {
	return strm << "V("<<vector.x<<","<< vector.y <<","<< vector.z <<")";
}
