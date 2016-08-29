#pragma once

#include<iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include "Shape.h"
#include <functional>

class Node {
    Shape shape;
    std::vector<Node *> children;
public:
    Node(Shape& shape, std::vector<Node *> children){
        this->shape = shape;
        this->children = children;
    }

    Node(const Node &other){
        this->shape = other.shape;
        this->children = other.children;
    }

    Shape & getShape(){
        return this->shape;
    }

    std::vector<Node *> getChildren(){
        return this->children;
    }

    void addChild(Node * child){
        this->children.push_back(child);
    }

};

class ShapeTree {
    Node *root;
public:
    ShapeTree();
    std::vector<Node *> applyRules(Node * current, std::vector<std::function<std::vector<Shape>(Shape)>> rules);
    std::vector<Node *> insert(Node *parent, std::vector<Shape> successors);

    std::vector<Shape> buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules, Shape& axiom);


};



