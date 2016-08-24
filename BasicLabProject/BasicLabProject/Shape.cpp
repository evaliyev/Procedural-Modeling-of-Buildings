#include "Shape.h"
#include "Vector3D.h"
#include <string>
Shape::Shape(std::string name, Vector3D& scopePosition, Vector3D& size) {
	this->name = name;
	this->scopePosition = scopePosition;
	this->size = size;
}

Shape& Shape::tranlate(Vector3D& translation) {
	this ->scopePosition.add( translation);
	return *this;
}

Shape& Shape::rotate(Vector3D& angles) {
	//TODO finish this function
}

Shape& Shape::setSize(Vector3D& newSize) {
	this->size = newSize;
	return *this;
}

Shape& Shape::pop(){
	//TODO finish this function
}

Shape& Shape::push() {
	//TODO finish this function
}

//TODO implement rest of functions 

std::ostream& operator<<(std::ostream &strm, const Shape &shape){
	return strm << "Shape("<<shape.name<< "," << shape.scopePosition <<","<<shape.size<<")";
}
