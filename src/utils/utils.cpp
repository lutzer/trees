//
//  utils.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "utils.hpp"
#include <math.h>

double utils::constrainAngle(double x){
    x = fmod(x,M_PI*2);
    while (x < 0) {
        x += M_PI * 2;
    }
    return x;
}

void utils::setNormals( ofMesh &mesh ){

    //The number of the vertices
    int nV = mesh.getNumVertices();

    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;

    vector<ofPoint> norm( nV ); //Array for the normals

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {

        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );

        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );

        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();

        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }

    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }

    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
}
