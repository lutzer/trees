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
#include <array>

namespace utils {

    #pragma mark - Math Helpers

    /// constrains an angle within 0 and 2_PI
    double constrainAngle(double x);

    int constrainRange(int val, int min, int max);

    /// Normalizes the given array.
    template<typename TYPE, std::size_t SIZE>
    void normalize(std::array<TYPE,SIZE> &bins) {
        // find maximum
        TYPE max = 0;
        for (TYPE val : bins) {
            max = std::max(val,max);
        }
        // normalize
        for (TYPE &val : bins) {
            val = val/max;
        }
    }

    #pragma mark - Drawing Helpers

    /// adds normals to a triangle mesh
    void setNormals(ofMesh &mesh);

}

#endif /* utils_hpp */
