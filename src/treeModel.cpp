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

    addBranchesToMesh(mesh, ofPoint( tree.origin.x, tree.origin.y, 0), 0, tree.base);
    return mesh;
}

void addBranchesToMesh(ofMesh &mesh, ofPoint origin, double angle, trees::Branch branch) {

    // add root of branch
    mesh.addVertex(origin);

    double newAngle = utils::constrainAngle(angle + branch.angle);


    // add end of branch
    ofPoint endpoint = ofPoint(origin.x + cos(newAngle) * branch.length, 0, origin.y + sin(newAngle) * branch.length);
    mesh.addVertex(endpoint);

    for (trees::Branch child : branch.children) {
        addBranchesToMesh(mesh,endpoint,newAngle,child);
    }
}
