#include"Parser.h"
#include "Shape.h"
#include "Visualizer.h"
#include "ShapeTree.h"

using namespace std;


int main(int argc, char** argv) {
	Parser parser("rules.txt");
	auto rules = parser.parseRules();
	ShapeTree tree;
	tree.buildTree(rules, Shape("main", Vector3D(0,0,0), Vector3D(40, 120, 80), SCOPE));
	auto nodes = tree.getLeafNodes();

	drawShapes(nodes, argc, argv);
	//drawDerivationTree(tree, argc, argv);
	return 0;
}