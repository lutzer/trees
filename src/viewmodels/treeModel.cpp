//
//  TreeMesh.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "ofMain.h"
#include "treeModel.hpp"
#include "utils.hpp"
#include "random.hpp"

// declare private methods
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

    // add root of branch
    mesh.addVertex(origin);

    double newAngle = angle + branch.angle;

    // add end of branch
    ofPoint endPoint = ofPoint(origin.x + cos(newAngle) * branch.length,
                               origin.y + sin(newAngle) * branch.length,
                               0);
    mesh.addVertex(endPoint);

    for (trees::Branch child : branch.children) {
        auto childOrigin = ofPoint(origin.x + (endPoint.x - origin.x) * child.position,
                                   origin.y + (endPoint.y - origin.y) * child.position,
                                   0);
        addBranchesToMesh(mesh, childOrigin, newAngle, child);
    }
}
