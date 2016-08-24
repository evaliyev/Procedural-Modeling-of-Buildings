#pragma once
#include<iostream>
#include <vector>

struct Vector3D {
	double x;
	double y;
	double z;
};

class Shape {
	std::string name;
	Vector3D scopePosition;
	Vector3D size;//size of the scope

	Shape(std::string name, Vector3D scopePosition, Vector3D size);

	/* Adds translation vector to the scope position*/
	Shape tranlate(Vector3D translation);

	/* Rotates the respective axis of the coordinate system */
	Shape rotate(Vector3D angles);

	/* Sets the size of the scope */
	Shape setSize(Vector3D newSize);

	/* Pops the current scope on a stack */
	Shape pop();

	/* Pushes the current scope on a stack */
	Shape push();

	/* Adds an instance of a geometric primitive with identifier objId */
	Shape getInstance(std::string objId);

	/* Split rule splits the current scope along one axis.
	Example: fac -> Subdiv(”Y”,3.5,0.3,3,3,3){ floor | ledge | floor | floor | floor } */
	std::vector<Shape> split(int axis, double* ratios, std::string * newShapeNames, int numberOfNewShapes);

	/* To allow for larger scale changes in the split rules, we often want to tile a specified element.
	Example :floor -> Repeat(”X”, 2) { B } */
	std::vector<Shape> repeat(int axis, int times, std::string newShapesNames);

	/* Splits the shape into shapes of lesser dimensions :
	a -> Comp(type, param) { A | B | ... | Z }
	Where type identifies the type of the component split with associated parameters param(if any).*/
	std::vector<Shape> componentSplit(std::string type, int param, std::string * newShapeNames);
};