//
//  TreeMesh.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "TreeModel.hpp"
#include "ofMain.h"

// declare private methods
void addBranchesToMesh(ofMesh &mesh, ofPoint origin, trees::Branch branch);

TreeModel::TreeModel() {
}

TreeModel::TreeModel(trees::Tree tree) {
    this->tree = tree;
}

ofMesh TreeModel::getMesh() {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINES);
    addBranchesToMesh(mesh,ofPoint(tree.origin.x,tree.origin.y,0),tree.base);
    return mesh;
}

void addBranchesToMesh(ofMesh &mesh, ofPoint origin, trees::Branch branch) {

    // add root of branch
    mesh.addVertex(origin);

    // add end of branch
    ofPoint endpoint = ofPoint(origin.x + cos(branch.angle) * branch.length, 0, origin.y + sin(branch.angle) * branch.length);
    mesh.addVertex(endpoint);

    for (trees::Branch child : branch.children) {
        addBranchesToMesh(mesh,endpoint,child);
    }
}
