//
//  binMesh.hpp
//  trees
//
//  Created by Lutz on 11/12/16.
//
//

#ifndef binMesh_hpp
#define binMesh_hpp

#include <stdio.h>
#include "ofMesh.h"

class BinMesh {

public:

    BinMesh();
    BinMesh(float * data, int rows, int columns);
    ofMesh getMesh(ofPoint pos, ofVec3f binSize);

private:
    float *bins;
    int rows,columns;


};

#endif /* binMesh_hpp */
