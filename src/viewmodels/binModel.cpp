//
//  binMesh.cpp
//  trees
//
//  Created by Lutz on 11/12/16.
//
//

#include "binModel.hpp"

static const int OPACITY = 100;


BinModel::BinModel(float* data, int rows, int columns) {
    this->binData = data;
    this->rows =  rows;
    this->columns = columns;
}

ofMesh BinModel::getMesh(ofPoint origin, ofVec3f size) {

    ofMesh mesh;

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    ofVec3f binSize = ofVec3f(size.x / rows, 0, size.z / columns);

    for (int i=0; i < rows*columns; i++) {

        ofPoint p1 = ofPoint(origin.x + (i % columns) * binSize.x, origin.y + binSize.y, origin.z + (i / rows) * binSize.z);

        ofColor color(int(max/binData[i] * 255),0,0,OPACITY);

        // draw first triangle
        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);

        // draw second triangle
        mesh.addVertex(p1);
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);
        mesh.addVertex(ofPoint(p1.x + binSize.x,p1.y,p1.z + binSize.z));
        mesh.addColor(color);

    }

    return mesh;
}
