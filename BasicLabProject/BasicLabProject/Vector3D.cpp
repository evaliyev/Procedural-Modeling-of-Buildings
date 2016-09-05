#include "Vector3D.h"

Vector3D::Vector3D(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
};

Vector3D Vector3D::copy() {
    return Vector3D(this->x, this->y, this->z);
}

float Vector3D::getElement(int axis) {
    switch (axis) {
        case 0:
            return this->x;
        case 1:
            return this->y;
        case 2:
            return this->z;
        default:
            throw std::invalid_argument("No such axis!");
    }
}

void Vector3D::setElement(int axis, float value) {
    switch (axis) {
        case 0:
            this->x = value;
            break;
        case 1:
            this->y = value;
            break;
        case 2:
            this->z = value;
            break;
        default:
            throw std::invalid_argument("No such axis!");
    }
}

void Vector3D::add(Vector3D& that) {
    this->x += that.x;
    this->y += that.y;
    this->z += that.z;
}

float Vector3D::getX() { return this->x; }

float Vector3D::getY() { return this->y; }

float Vector3D::getZ() { return this->z; }

void Vector3D::set(Vector3D &vector) {
    this->x = vector.getX();
    this->y = vector.getY();
    this->z = vector.getZ();
}

std::ostream &operator<<(std::ostream &strm, const Vector3D &vector) {
    return strm << "V(" << vector.x << "," << vector.y << "," << vector.z << ")";
}
