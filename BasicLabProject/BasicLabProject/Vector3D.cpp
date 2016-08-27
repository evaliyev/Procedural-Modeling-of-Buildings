#include "Vector3D.h"


Vector3D::Vector3D(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
};

Vector3D Vector3D::copy() {
    return Vector3D(this->x, this->y, this->z);
}

 
void Vector3D::add(Vector3D that) {
	this->x += that.x;
	this->y += that.y;
	this->z += that.z;
}

void Vector3D::add(float ratio) {
    this->x += ratio;
    this->y += ratio;
    this->z += ratio;
}

float Vector3D::getX() { return this->x; }

float Vector3D::getY() { return this->y; }

float Vector3D::getZ() { return this->z; }

void Vector3D::set(Vector3D& vector) {
    this->x = vector.getX();
    this->y = vector.getY();
    this->z = vector.getZ();
}

void Vector3D::multiply(float ratio) {
    this->x = this->x*ratio;
    this->y = this->y*ratio;
    this->z = this->z*ratio;
}

void Vector3D::multiply(Vector3D &that) {
    this->x = this->x*that.x;
    this->y = this->y*that.y;
    this->z = this->z*that.z;
}

std::ostream& operator<<(std::ostream &strm, const Vector3D &vector) {
	return strm << "V("<<vector.x<<","<< vector.y <<","<< vector.z <<")";
}
