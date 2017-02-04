//
//  binMesh.cpp
//  trees
//
//  Created by Lutz on 11/12/16.
//
//

#include "binModel.hpp"

static const float COLOR_ALPHA = 100;
static const float DEFORMATION_FACTOR = 20.0;

///  Adds normals to a mesh.
void setNormals(const ofMesh &mesh);

BinModel::BinModel() {
    this->binData = NULL;
    this->rows =  0;
    this->columns = 0;
}

BinModel::BinModel(float *data, int columns, int rows) {
    this->binData = data;
    this->rows =  rows;
    this->columns = columns;
}

ofMesh BinModel::getMesh(ofPoint origin, ofVec3f size) {
    ofMesh mesh;

    //mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int meshIndex = 0;

    ofVec3f binSize = ofVec3f(size.x / columns, size.y / rows, 0);
    for (int i = 0; i < rows * columns; i++) {
        const auto x = origin.x + (i % columns) * binSize.x;
        const auto y = origin.y + (i / columns) * binSize.y;
        const auto z = origin.z + binSize.z;
        ofPoint p1 = ofPoint(x, y, z);

        ofColor color = ofColor(binData[i] * 255, binData[i] * 255, 0, COLOR_ALPHA);

        // Add 4 points for one bin.
        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x, p1.y, p1.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x, p1.y + binSize.y, p1.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x, p1.y + binSize.y, p1.z));
        mesh.addColor(color);

        // Add 2 triangles per bin.
        mesh.addTriangle(meshIndex, meshIndex + 1, meshIndex + 3);
        mesh.addTriangle(meshIndex, meshIndex + 2, meshIndex + 3);

        meshIndex += 4;
    }
    return mesh;
}
