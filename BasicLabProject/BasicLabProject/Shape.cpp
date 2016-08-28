#include "Shape.h"
#include "Vector3D.h"
#include <string>
#include "math.h"
#include <iostream>

const float PI = 3.14;

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

Vector3D Shape::rotate_around_axis(Vector3D &position, float degrees, int axis) {
    float angle = PI * degrees / 180.0;
    float cos_angle = cos(angle);
    float sin_angle = sin(angle);
    float posX = position.getX();
    float posY = position.getY();
    float posZ = position.getZ();
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
            y = -sin_angle * posX + cos_angle * posZ;
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
    Vector3D rotatedX = rotate_around_axis(this->scopePosition, angles.getX(), 0);
    Vector3D rotatedY = rotate_around_axis(rotatedX, angles.getY(), 1);
    Vector3D rotatedZ = rotate_around_axis(rotatedY, angles.getZ(), 2);
    this->scopePosition.set(rotatedZ);
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

std::vector<Shape> Shape::repeat(int axis, int times, std::string newShapesNames) {
    std::vector<Shape> successors;
    float ratio = this->size.getElement(axis)/times;

    Vector3D newPosition = this->scopePosition.copy();
    Vector3D newSize = this->size.copy();
    float positionChange = this->scopePosition.getElement(axis);

    for(int i=0; i<times; i++){
        positionChange += i*ratio;
        newPosition.setElement(axis,positionChange);
        newSize.setElement(axis,ratio);
        Shape newShape(newShapesNames,newPosition,newSize, this->type);
        successors.push_back(newShape);
    }

    return successors;
}

std::ostream &operator<<(std::ostream &strm, const Shape &shape) {
    return strm << "Shape(" << shape.name << "," << shape.scopePosition << "," << shape.size << ")";
}


