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
        std::vector<Shape> result = rules[i](current->shape);
        successors.insert(result.begin(), result.end(), successors.begin());
    }


    for (int i = 0; i < successors.size(); i++) {
        Node *child;
        child->shape = successors[i];
        current->children.push_back(child);
    }

    return current->children;
}


std::vector<Shape> ShapeTree::buildTree(std::vector<std::function<std::vector<Shape>(Shape)>> rules, Shape &axiom) {
    std::vector<Shape> leafShapes;
    std::deque<Node *> shapeQueue;
    Node *current;
    current->shape = axiom;
    this->root = current;
    shapeQueue.push_back(current);

    while (!shapeQueue.empty()) {
        shapeQueue.pop_front();
        std::vector<Node *> children = applyRules(current, rules);
        for (int j = 0; j < children.size(); j++) {
            if (children[j]->shape.getType() == SCOPE)
                shapeQueue.push_back(children[j]);

            else if (children[j]->shape.getType() != INACTIVE)
                leafShapes.push_back(children[j]->shape);
        }
        current->shape.setType(INACTIVE);
        current = shapeQueue.at(0);
    }
    return leafShapes;
}



