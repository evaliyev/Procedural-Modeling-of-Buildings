//
// Created by mari on 28.08.16.
//

#include "ShapeTree.h"
#include<iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include "Shape.h"

ShapeTree::ShapeTree() {
    this->root = NULL;
}

Node* ShapeTree::insert(Shape &shape, Node *parent) {
    Node* currentNode;
    currentNode->currentShape = shape;
    currentNode->parent = parent;
    if(this->root == NULL)
        this->root = currentNode;
    else
        parent->children.push_back(currentNode);
    return currentNode;
}

std::vector<Shape> ShapeTree::buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules,Shape& axiom) {


}

