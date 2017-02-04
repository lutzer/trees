//
//  environment.hpp
//  trees
//
//  Created by Lutz on 04/02/17.
//
//

#ifndef environment_hpp
#define environment_hpp

#include <stdio.h>
#include <vector>

#include "point.hpp"

namespace env {

    static const float BIN_SIZE = 2;
    static const double DENSITY_MULTIPLIER = 0.1; // Multiplied with branch thickness to sum up density values.

    using BinArray = std::vector<float>;

    struct Bins {
        BinArray densities;
        BinArray light;
        pts::SizeInt size;

    };

    struct Environment {

        pts::Point sun;
        pts::BoundingBox boundingBox;

        Environment(pts::Point sun, pts::BoundingBox boundingBox);
    };

    /// calculates the size of the bin matrix from the boundingBox dimensions, so that every bin is a square
    pts::SizeInt calculateBinMatrixSize(pts::BoundingBox boundingBox);

}

#endif /* environment_hpp */
