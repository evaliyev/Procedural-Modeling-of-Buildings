#pragma once

#include<iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include "Shape.h"
#include <functional>

struct Node {
    Shape currentShape;
    Node *parent;
    std::vector<Node *> children;
};

class ShapeTree {
    Node *root;
public:
    ShapeTree();
    Node* insert(Shape& shape, Node* parent);

    static std::vector<Shape> buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules, Shape& axiom);

};



