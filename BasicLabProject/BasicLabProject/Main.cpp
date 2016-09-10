#include"Parser.h"
#include "Shape.h"
#include "Visualizer.h"
#include "ShapeTree.h"

using namespace std;


int main(int argc, char** argv) {
	 
	
	 //auto shapes = rules[0](Shape("X", Vector3D(20, 0, 20), Vector3D(0, 0, 0),SCOPE));
	 //drawShapes(shapes, argc, argv);

	 //Shape testShape("main", Vector3D(20, 0, 20), Vector3D(10, 10, 60), SCOPE);
	 //vector<float> ratios = { 0.25,0.25,0.25,0.25 };
	// vector<string> shapes = { "A","B","C","D" };
//	 auto result = testShape.split(2, ratios, shapes);
	// for (auto x : result)
	//	 std::cout << x << endl;

 

	Parser parser("rules.txt");
	auto rules = parser.parseRules();
	ShapeTree tree;
	tree.buildTree(rules, Shape("main", Vector3D(0,0,0), Vector3D(100, 60, 100), SCOPE));
	auto nodes = tree.getLeafNodes();

	drawShapes(nodes, argc, argv);
	//drawDerivationTree(tree, argc, argv);
	return 0;
}