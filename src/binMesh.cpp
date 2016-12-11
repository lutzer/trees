//
//  binMesh.cpp
//  trees
//
//  Created by Lutz on 11/12/16.
//
//

#include "binMesh.hpp"

static const int OPACITY = 100;


BinMesh::BinMesh(float* data, int rows, int columns) {
    this->bins = data;
    this->rows =  rows;
    this->columns = columns;
}

ofMesh BinMesh::getMesh(ofPoint pos, ofVec3f binSize) {

    ofMesh mesh;

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    for (int i=0; i < rows*columns; i++) {
        ofPoint p1 = ofPoint(pos.x + (i % rows) * binSize.x, pos.y, pos.z + (i / rows) * binSize.z);

        ofColor color(255,0,0,OPACITY);

        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);

        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);

    }

    return mesh;
}
