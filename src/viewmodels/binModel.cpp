//
//  binMesh.cpp
//  trees
//
//  Created by Lutz on 11/12/16.
//
//

#include "binModel.hpp"

static const int OPACITY = 100;
static const float DEFORMATION_FACTOR = 20.0;

/// adds normals to a mesh
void setNormals( ofMesh &mesh );

BinModel::BinModel() {
    this->binData = NULL;
    this->rows =  0;
    this->columns = 0;
}


BinModel::BinModel(float* data, int rows, int columns) {
    this->binData = data;
    this->rows =  rows;
    this->columns = columns;
}

ofMesh BinModel::getMesh(ofPoint origin, ofVec3f size) {

    ofMesh mesh;

    //mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int meshIndex = 0;

    ofVec3f binSize = ofVec3f(size.x / rows, size.y / columns, 0);
    for (int i=0; i < rows*columns; i++) {

        ofPoint p1 = ofPoint(origin.x + (i % columns) * binSize.x, origin.y + (i / rows) * binSize.y, origin.z + binSize.z);

        ofColor color(binData[i] * 255,0,0,OPACITY);

        // add 4 points for one bin
        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x, p1.y, p1.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x, p1.y + binSize.y, p1.z ));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x, p1.y  + binSize.y, p1.z));
        mesh.addColor(color);

        // add 2 triangles per bin
        mesh.addTriangle(meshIndex, meshIndex +1, meshIndex +3);
        mesh.addTriangle(meshIndex, meshIndex +2, meshIndex +3);

        meshIndex += 4;
    }


    return mesh;
}
