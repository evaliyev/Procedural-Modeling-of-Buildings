#include<iostream>
//#include<conio.h>
#include"Parser.h"
#include "Shape.h"
#include "Visualizer.h"
#include "ShapeTree.h"

using namespace std;

int main(int argc, char** argv) {
	 
	Parser parser("rules.txt");
	auto rules = parser.parseRules();
	auto shapes = rules[0](Shape("A", Vector3D(0, 0, 0), Vector3D(0, 0, 0),SCOPE));
	
	ShapeTree tree;
	tree.buildTree(rules, Shape("A", Vector3D(0, 0, 0), Vector3D(0, 0, 0), SCOPE));
	auto nodes = tree.getLeafNodes();
	//drawShapes(shapes,argc,argv);
	drawDerivationTree(tree, argc, argv);
	//_getch();
	return 0;
}