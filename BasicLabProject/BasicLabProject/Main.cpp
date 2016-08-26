#include<iostream>
#include<conio.h>
#include "Shape.h"
#include "Visualizer.h"

using namespace std;

int main(int argc, char** argv) {
	 
	Shape shape("floor", Vector3D(0,0,0), Vector3D(0, 0, 0));
	cout << shape.setSize(Vector3D(1,1,1));
	 
	start(argc,argv);

	_getch();
	return 0;
}