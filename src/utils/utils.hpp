//
//  utils.hpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include "ofMain.h"

namespace utils {

    #pragma mark - Math Helpers

    /// constrains an angle within 0 and 2_PI
    double constrainAngle(double x);

    #pragma mark - Drawing Helpers

    /// adds normals to a triangle mesh
    void setNormals(ofMesh &mesh);

}

#endif /* utils_hpp */
