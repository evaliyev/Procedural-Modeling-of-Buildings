#include<iostream>
#include<conio.h>
#include "Shape.h"

using namespace std;

int main(int argc, char** argv) {
	 
	Shape shape("floor", Vector3D(0,0,0), Vector3D(0, 0, 0));
	cout << shape.setSize(Vector3D(1,1,1));
	_getch();
	return 0;
}