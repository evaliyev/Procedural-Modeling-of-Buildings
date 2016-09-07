//
// Created by mari on 28.08.16.
//

#include "ShapeTree.h"
#include<iostream>
#include <vector>
#include "Vector3D.h"
#include "Type.h"
#include "Shape.h"
#include <deque>


ShapeTree::ShapeTree() {
    this->root = NULL;
}

std::vector<Node *> ShapeTree::applyRules(Node *current, std::vector<std::function<std::vector<Shape>(Shape)>> rules) {
    std::vector<Shape> successors;
    for (int i = 0; i < rules.size(); i++) {
        std::vector<Shape> result = rules[i](current->getShape());
        successors.reserve(successors.size() + result.size());
        successors.insert(successors.end(), result.begin(), result.end());
    }
    for (int i = 0; i < successors.size(); i++) {
        std::vector<Node *> children;
        Node *child = new Node(successors[i], children);
        current->addChild(child);
    }
    return current->getChildren();
}

void ShapeTree::buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules, Shape &axiom) {
    std::deque<Node *> shapeQueue;
    std::vector<Node *> children;
    Node *current = new Node(axiom, children);
    this->root = current;
    shapeQueue.push_back(current);
    //std::cout<<shapeQueue.size()<<std::endl;

    while (!shapeQueue.empty()) {
		current = shapeQueue.front();
        //std::cout<<shapeQueue.size()<<std::endl;
        //std::cout<<current->getShape()<<std::endl;

        std::vector<Node *> children = applyRules(current, rules);
        for (int j = 0; j < children.size(); j++) {
            //std::cout<<children[j]->getShape()<<std::endl;
            if (children[j]->getShape().getType() == SCOPE)
                shapeQueue.push_back(children[j]);

            else if (children[j]->getShape().getType() != INACTIVE)
                this->leafNodes.push_back(children[j]->getShape());
        }
        current->getShape().setType(INACTIVE);
		shapeQueue.pop_front();
    }
	return;
}

std::vector<Shape> ShapeTree::getLeafNodes(){
	return this->leafNodes;
}

Node* ShapeTree::getRoot() {
	return  this->root;
};



