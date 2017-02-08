//
//  TreeMesh.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "treeModel.hpp"

#include "ofMain.h"
#include "utils.hpp"
#include "random.hpp"

// Declare private methods.
void addBranchesToMesh(ofMesh &mesh, ofPoint origin, double angle, const trees::Branch &branch, bool enableGravity);

TreeModel::TreeModel(trees::Tree &tree) {
    this->tree = tree;
    this->enableGravity = false;
}

TreeModel::TreeModel(trees::Tree &tree, bool enableGravity) {
    this->tree = tree;
    this->enableGravity = enableGravity;
}

ofMesh TreeModel::getMesh() {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINES);

    addBranchesToMesh(mesh, ofPoint(tree.origin.x, tree.origin.y, 0), 0, tree.base, this->enableGravity);

    return mesh;
}

void addBranchesToMesh(ofMesh &mesh, ofPoint origin, double angle, const trees::Branch &branch, bool enableGravity) {
    // Add root of branch.
    mesh.addVertex(origin);

    double newAngle = angle + branch.angle;
    if (enableGravity) {
        newAngle += branch.gravitationalAngleChange;
    }

    std::cout << branch.gravitationalAngleChange << endl;

    // Add end of branch.
    ofPoint endPoint = ofPoint(origin.x + cos(newAngle) * branch.length,
                               origin.y + sin(newAngle) * branch.length,
                               0);
    mesh.addVertex(endPoint);

    for (trees::Branch child : branch.children) {
        const auto childX = origin.x + (endPoint.x - origin.x) * child.position;
        const auto childY = origin.y + (endPoint.y - origin.y) * child.position;
        const auto childOrigin = ofPoint(childX, childY, 0);
        addBranchesToMesh(mesh, childOrigin, newAngle, child, enableGravity);
    }
}
