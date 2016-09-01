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
	//auto shapes = rules[0](Shape("main", Vector3D(20, 0, 20), Vector3D(10, 60, 10),SCOPE));
	
	ShapeTree tree;
	tree.buildTree(rules, Shape("main", Vector3D(20, 0, 20), Vector3D(10, 60, 10), SCOPE));
	auto nodes = tree.getLeafNodes();
	 drawShapes(nodes,argc,argv);
	//drawDerivationTree(tree, argc, argv);
	//_getch();
	return 0;
}