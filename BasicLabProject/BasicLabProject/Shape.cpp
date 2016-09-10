#include "Shape.h"
#include "Vector3D.h"
#include <string>
#include "math.h"


const float PI = 3.14159265358979323846;

Shape::Shape(std::string name, Vector3D &scopePosition, Vector3D &size, Type type) {
    this->name = name;
    this->scopePosition = scopePosition;
    this->size = size;
    this->type = type;
}

Shape &Shape::translate(Vector3D &translation) {
    this->scopePosition.add(translation);
    return *this;
}

Vector3D& Shape::getScopePosition() { return this->scopePosition; }

Vector3D& Shape::getSize() { return this->size; }

Type Shape::getType() {return this->type; }

void Shape::setType(Type type) {this->type = type; }

Vector3D Shape::rotate_around_axis(Vector3D &size, float degrees, int axis) {
    float angle = PI * degrees / 180.0;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float posX = size.getX();
    float posY = size.getY();
    float posZ = size.getZ();
    float x, y, z; 
    switch (axis) {
        case 0: {
            x = cos_angle * posX - sin_angle * posY;
            y = sin_angle * posX + cos_angle * posY;
            z = posZ;
        }
            break;
        case 1: {
            x = cos_angle * posX + sin_angle * posZ;
            y = posY;
            z = -sin_angle * posX + cos_angle * posZ;
        }
            break;
        case 2: {
            x = posX;
            y = cos_angle * posY - sin_angle * posZ;
            z = sin_angle * posY + cos_angle * posZ;
        }
        default:
            break;
    }

    return Vector3D(x, y, z);

}

Shape &Shape::rotate(Vector3D &angles) {
    Vector3D rotatedX = rotate_around_axis(this->size, angles.getX(), 0);
    Vector3D rotatedY = rotate_around_axis(rotatedX, angles.getY(), 1);
    Vector3D rotatedZ = rotate_around_axis(rotatedY, angles.getZ(), 2);
    this->size.set(rotatedZ);
    return *this;
}

Shape &Shape::setSize(Vector3D &newSize) {
    this->size = newSize;
    return *this;
}

std::vector<Shape> Shape::split(int axis, std::vector<float> ratios, std::vector<std::string> newShapeNames) {
    std::vector<Shape> successors;
    if (ratios.size() != newShapeNames.size()) {
        std::cout << "Illegal split, num of ratios and names should be the same" << std::endl;
        successors.push_back(*this);
        return successors;
    }
    Vector3D newPosition = this->scopePosition.copy();;
    Vector3D newSize = this->size.copy();
    float positionChange = this->scopePosition.getElement(axis);

    for (int i = 0; i < ratios.size(); i++) {
        newSize.setElement(axis, ratios[i]);
        if (i != 0)
            positionChange += ratios[i - 1];
        newPosition.setElement(axis, positionChange);
        Shape newShape = Shape(newShapeNames[i], newPosition, newSize, this->type);
        successors.push_back(newShape);
    }
    return successors;
}

void Shape::push(std::stack<Shape> shapes, Shape &shape) {
    shapes.push(shape);
    //shape.name or no_name?
}

Shape &Shape::pop(std::stack<Shape> shapes) {
    Shape& old = shapes.top();
    shapes.pop();
    return old;
}

std::string Shape::getName() { return this->name; }

void Shape::setName(std::string name){
	this->name = name;
}

std::vector<Shape> Shape::repeat(int axis, int times, std::vector<std::string> newShapesNames) {
    std::vector<Shape> successors;
    float ratio = this->size.getElement(axis)/times;

    Vector3D newPosition = this->scopePosition.copy();
    Vector3D newSize = this->size.copy();
	
	float initPosition = this->scopePosition.getElement(axis);
    for(int i=0; i<times; i++){
        newPosition.setElement(axis, initPosition + i*ratio);
        newSize.setElement(axis,ratio);
        Shape newShape(newShapesNames[i],newPosition,newSize, this->type);
        successors.push_back(newShape);
    }

    return successors;
}

std::vector<Shape> Shape::realRepeat(int axis, int times, std::string newShapesName) {
	std::vector<Shape> successors;

	Vector3D newPosition = this->scopePosition.copy();
	Vector3D newSize = this->size.copy();

	float initPosition = this->scopePosition.getElement(axis);
	for (int i = 0; i<times; i++) {
		
		Shape newShape(newShapesName, newPosition, newSize, this->type);
		std::cout << newShape << std::endl;
		successors.push_back(newShape);
		newPosition.setElement(axis,newPosition.getElement(axis)+this->size.getElement(axis));
		std::cout << newPosition << std::endl;
		
	}

	return successors;
}

std::vector<Shape> Shape::componentSplit(std::string type, std::vector<std::string> newShapeNames) {
	std::vector<Shape> newShapes;
	auto position = this->getScopePosition();
	auto size = this->getSize();
	
	//front
	auto fp = position.copy();
	auto fs = size.copy();
	fs.setElement(0,0);
	newShapes.push_back(Shape(newShapeNames[0],fp,fs,SCOPE));
	//back
	auto bp = position.copy();
	auto bs = size.copy();
	bp.setElement(0,bp.getX()+ bs.getX());
	bs.setElement(0, 0);
	newShapes.push_back(Shape(newShapeNames[1], bp, bs, SCOPE));
	//left
	auto lp = position.copy();
	auto ls = size.copy();
	ls.setElement(2, 0);
	std::cout << "l " << ls << std::endl;
	std::cout << lp << std::endl;
	newShapes.push_back(Shape(newShapeNames[2], lp, ls, SCOPE));
	//right
	auto rp = position.copy();
	auto rs = size.copy();
	rp.setElement(2, rp.getZ()+rs.getZ());
	rs.setElement(2, 0);
	newShapes.push_back(Shape(newShapeNames[3], rp, rs, SCOPE));

	if (type.compare("sides") != 0) {
	//top
		auto tp = position.copy();
		auto ts = size.copy();
		tp.setElement(1, tp.getY()+ ts.getY());
		ts.setElement(1, 0);
		newShapes.push_back(Shape(newShapeNames[4], tp, ts, SCOPE));
	//bottom
		auto bbp = position.copy();
		auto bbs = size.copy();
		bbs.setElement(1, 0);
		newShapes.push_back(Shape(newShapeNames[5], bbp, bbs, SCOPE));
	}
	return newShapes;
}

std::ostream &operator<<(std::ostream &strm, const Shape &shape) {
    return strm << "Shape(" << shape.name << "," << shape.scopePosition << "," << shape.size << "," << shape.type << ")";
}


