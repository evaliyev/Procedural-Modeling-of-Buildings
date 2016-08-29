#pragma once

#include<iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include "Shape.h"
#include <functional>

struct Node {
    Shape shape;
    std::vector<Node *> children;
};

class ShapeTree {
    Node *root;
public:
    ShapeTree();
    std::vector<Node *> applyRules(Node * current, std::vector<std::function<std::vector<Shape>(Shape)>> rules);
    std::vector<Node *> insert(Node *parent, std::vector<Shape> successors);

    std::vector<Shape> buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules, Shape& axiom);


};



