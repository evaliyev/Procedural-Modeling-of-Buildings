#pragma once

#include <iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include <stack>

class Shape {
    std::string name;
    Vector3D scopePosition;
    Vector3D size;
    //size of the scope
    Type type; //scope/non-scope
    friend std::ostream &operator<<(std::ostream &strm, const Shape &shape);

    Vector3D rotate_around_axis(Vector3D &position, float degrees, int axe);

public:
    Shape(std::string name, Vector3D &scopePosition, Vector3D &size, Type type);

    /* Adds translation vector to the scope position*/
    Shape &translate(Vector3D &translation);

    /* Rotates the respective axis of the coordinate system */
    Shape &rotate(Vector3D &angles);

    /* Sets the size of the scope */
    Shape &setSize(Vector3D &newSize);

    /* Pops the current scope on a stack */
    static Shape &pop(std::stack<Shape> shapes);

    /* Pushes the current scope on a stack */
    static void push(std::stack<Shape> shapes, Shape &shape);

    void setType(Type type);

    Type getType();

    Vector3D& getScopePosition();

    Vector3D& getSize();

    std::string getName();

    /* Adds an instance of a geometric primitive with identifier objId */
    Shape getInstance(std::string objId);

    /* Split rule splits the current scope along one axis.
    Example: fac -> Subdiv(�Y�,3.5,0.3,3,3,3){ floor | ledge | floor | floor | floor } */
    std::vector<Shape> split(int axis, std::vector<double> ratios, std::vector<std::string> newShapeNames);

    /* To allow for larger scale changes in the split rules, we often want to tile a specified element.
    Example :floor -> Repeat(�X�, 2) { B } */
    std::vector<Shape> repeat(int axis, int times, std::string newShapesNames);

    /* Splits the shape into shapes of lesser dimensions :
    a -> Comp(type, param) { A | B | ... | Z }
    Where type identifies the type of the component split with associated parameters param(if any).*/
    std::vector<Shape> componentSplit(std::string type, int param, std::string *newShapeNames);

};

/* Analog to Java.lang.Object.toString() */
std::ostream &operator<<(std::ostream &strm, const Shape &shape);