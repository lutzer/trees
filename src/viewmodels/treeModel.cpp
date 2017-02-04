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
void addBranchesToMesh(ofMesh &mesh, ofPoint origin, double angle, trees::Branch branch);

TreeModel::TreeModel() {
}

TreeModel::TreeModel(trees::Tree tree) {
    this->tree = tree;
}

ofMesh TreeModel::getMesh() {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINES);

    trees::Branch emptyBranch = {};

    addBranchesToMesh(mesh, ofPoint(tree.origin.x, tree.origin.y, 0), 0, tree.base);
    return mesh;
}

void addBranchesToMesh(ofMesh &mesh, ofPoint origin, double angle, trees::Branch branch) {
    // Add root of branch.
    mesh.addVertex(origin);

    double newAngle = angle + branch.angle;

    // Add end of branch.
    ofPoint endPoint = ofPoint(origin.x + cos(newAngle) * branch.length,
                               origin.y + sin(newAngle) * branch.length,
                               0);
    mesh.addVertex(endPoint);

    for (trees::Branch child : branch.children) {
        const auto childX = origin.x + (endPoint.x - origin.x) * child.position;
        const auto childY = origin.y + (endPoint.y - origin.y) * child.position;
        const auto childOrigin = ofPoint(childX, childY, 0);
        addBranchesToMesh(mesh, childOrigin, newAngle, child);
    }
}
