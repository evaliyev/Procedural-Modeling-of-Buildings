#include "Shape.h"
#include "Vector3D.h"
#include <string>
#include "math.h"

Shape::Shape(std::string name, Vector3D& scopePosition, Vector3D& size, Type type) {
	this->name = name;
	this->scopePosition = scopePosition;
	this->size = size;
    this->type = type;

}

Shape& Shape::tranlate(Vector3D& translation) {
	this ->scopePosition.add( translation);
	return *this;
}

Vector3D Shape::rotate_around_axis(Vector3D& position, float degrees, int axe) {
    float angle = M_PI * degrees / 180.0;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float posX = position.getX();
    float posY = position.getY();
    float posZ = position.getZ();
    float x, y, z;
    switch(axe){
        case 0: {
            x = cos_angle*posX - sin_angle*posY;
            y = sin_angle*posX + cos_angle*posY;
            z = posZ;
        }
            break;
        case 1: {
            x = cos_angle*posX + sin_angle*posZ;
            y = posY;
            y = - sin_angle*posX + cos_angle*posZ;
        }
            break;
        case 2: {
            x = posX;
            y = cos_angle*posY - sin_angle*posZ;
            z = sin_angle*posY + cos_angle*posZ;
        }
        default:
            break;
    }

    return Vector3D(x,y,z);

}

Shape& Shape::rotate(Vector3D& angles) {
	Vector3D rotatedX = rotate_around_axis(this->scopePosition,angles.getX(),0);
    Vector3D rotatedY = rotate_around_axis(rotatedX,angles.getY(),1);
    Vector3D rotatedZ = rotate_around_axis(rotatedY,angles.getZ(),2);
    this->scopePosition.set(rotatedZ);
	return *this;
}


Shape& Shape::setSize(Vector3D& newSize) {
	this->size = newSize;
	return *this;
}

//Shape& Shape::pop(){
//	//TODO finish this function
//	return *this;
//}
//
//Shape& Shape::push() {
//	//TODO finish this function
//	return *this;
//}
//
////TODO implement rest of functions

std::ostream& operator<<(std::ostream &strm, const Shape &shape){
	return strm << "Shape("<<shape.name<< "," << shape.scopePosition <<","<<shape.size<<")";
}


