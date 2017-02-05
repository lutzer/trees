//
//  utils.hpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#ifndef utils_hpp
#define utils_hpp

#include "ofMain.h"

#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>

namespace utils {

    #pragma mark - Math Helpers

    /// Constrains an angle within 0 and 2_PI.
    double constrainAngle(double x);

    template<typename TYPE>
    ///  Adjusts value, so it's contained between min and max.
    int constrainRange(TYPE val, TYPE min, TYPE max);

    template<typename TYPE>
    /// Normalizes the given array.
    void normalize(std::vector<TYPE> &bins);

    #pragma mark - Drawing Helpers

    /// Adds normals to a triangle mesh.
    void setNormals(ofMesh &mesh);

    /// checks if point lies within the rectangle
    bool containsPoint(const ofRectangle &rectangle, const ofPoint &point);

    #pragma mark - Mapping Helpers

    // TODO: move the symbol definitions to .cpp file

    template<typename RETURN_TYPE, typename INPUT_TYPE, typename F>
    /// wraps std::transform function to iterate over vector
    std::vector<RETURN_TYPE> map(std::vector<INPUT_TYPE> array, F lambda) {
        std::vector<RETURN_TYPE> result;
        std::transform(array.begin(), array.end(), std::back_inserter(result), lambda);
        return result;
    }

    template<typename RETURN_TYPE, typename INPUT_TYPE, typename F>
    /// wraps std::accumulate function to fold a vector
    RETURN_TYPE fold(std::vector<INPUT_TYPE> array, RETURN_TYPE initial, F lambda) {
        return std::accumulate(array.begin(), array.end(), initial, lambda);
    }
}  // namespace utils

#endif /* utils_hpp */
