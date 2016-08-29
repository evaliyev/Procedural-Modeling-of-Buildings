#include<iostream>
//#include<conio.h>
#include"Parser.h"
#include "Shape.h"
#include "Visualizer.h"

using namespace std;

int main(int argc, char** argv) {
	 
	Parser parser("rules.txt");
	auto rules = parser.parseRules();
	auto shapes = rules[0](Shape("A", Vector3D(0, 0, 0), Vector3D(0, 0, 0),SCOPE));
	drawShapes(shapes,argc,argv);
	//_getch();
	return 0;
}