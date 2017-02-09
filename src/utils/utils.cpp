//
//  utils.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "utils.hpp"

#include <math.h>
#include <algorithm>
#include <vector>

double utils::constrainAngle(double x) {
    x = fmod(x, M_PI * 2);
    while (x < 0) {
        x += M_PI * 2;
    }
    return x;
}

template<>
int utils::constrainRange(int val, int min, int max) {
    return std::max(std::min(val, max), min);
}

template<>
float utils::constrainRange(float val, float min, float max) {
    return std::max(std::min(val, max), min);
}

// Float implementation of normalize.
template<>
void utils::normalize(std::vector<float> &bins) {
    // Find maximum.
    float max = 0;
    for (float val : bins) {
        max = std::max(val, max);
    }

    // Normalize.
    for (float &val : bins) {
        val = val / max;
    }
}

double utils::multiplyWithWeight(double value, double modifier, double weight) {
    return value * modifier * weight + value * (1.0 - weight);
}

void utils::setNormals(ofMesh &mesh) {
    // The number of vertices.
    const auto nV = mesh.getNumVertices();

    // The number of the triangles
    const auto nT = mesh.getNumIndices() / 3;

    vector<ofPoint> norm(nV);  // Array for the normals.

    // Scan all the triangles. For each triangle, add its normal to norm's
    // vectors of triangle's vertices.
    for (int t = 0; t < nT; t++) {
        // Get indices of the triangle t.
        const auto i1 = mesh.getIndex(3 * t);
        const auto i2 = mesh.getIndex(3 * t + 1);
        const auto i3 = mesh.getIndex(3 * t + 2);

        // Get vertices of the triangle.
        const ofPoint &v1 = mesh.getVertex(i1);
        const ofPoint &v2 = mesh.getVertex(i2);
        const ofPoint &v3 = mesh.getVertex(i3);

        // Compute the triangle's normal.
        ofPoint dir = ((v2 - v1).getCrossed(v3 - v1)).getNormalized();

        // Accumulate it to norm array for i1, i2, i3.
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }

    // Normalize the normal's length.
    for (int i = 0; i < nV; i++) {
        norm[i].normalize();
    }

    // Set the normals to mesh.
    mesh.clearNormals();
    mesh.addNormals(norm);
}


bool utils::containsPoint(const ofRectangle &rectangle, const ofPoint &point) {
    return point.x >= rectangle.getMinX() && point.x <= rectangle.getMaxX() && point.y >= rectangle.getMinY() && point.y <= rectangle.getMaxY();
}

ofMesh utils::createCylinder(const ofPoint &start, const ofPoint &end, float radius, int radiusSegments) {

    ofMesh mesh;

    ofPoint lengthVector = (end - start).normalize();

    //get any orthagonalVector to length vector
    ofPoint orthagonalVector = lengthVector.getPerpendicular(ofPoint(lengthVector.y,lengthVector.z,lengthVector.x));

    //create bottom circle
    double step = M_PI*2 / radiusSegments;
    for (int i=0; i < radiusSegments; i++) {
        mesh.addVertex(start);
        mesh.addVertex(start + orthagonalVector.getRotatedRad(i * step, lengthVector) * radius);
        mesh.addVertex(start + orthagonalVector.getRotatedRad((i+1) * step, lengthVector) * radius);
    }

    //create top circle
    for (int i=0; i < radiusSegments; i++) {
        mesh.addVertex(end);
        mesh.addVertex(end + orthagonalVector.getRotatedRad(i * step, lengthVector) * radius);
        mesh.addVertex(end + orthagonalVector.getRotatedRad((i+1) * step, lengthVector) * radius);
    }

    //create two connecting polygons per height segment
    for (int i=0; i < radiusSegments; i++) {
        mesh.addVertex(end + orthagonalVector.getRotatedRad(i * step, lengthVector) * radius);
        mesh.addVertex(end + orthagonalVector.getRotatedRad((i+1) * step, lengthVector) * radius);
        mesh.addVertex(start + orthagonalVector.getRotatedRad(i * step, lengthVector) * radius);

        mesh.addVertex(start + orthagonalVector.getRotatedRad(i * step, lengthVector) * radius);
        mesh.addVertex(start + orthagonalVector.getRotatedRad((i+1) * step, lengthVector) * radius);
        mesh.addVertex(end + orthagonalVector.getRotatedRad((i+1) * step, lengthVector) * radius);
    }
    
    return mesh;
}
