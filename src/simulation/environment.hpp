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
    /// The width and height of the bins in the bounding box.
    static const float BIN_SIZE = 2;

    /// Defines the extent to which branches contribute to density values.
    static const double DENSITY_MULTIPLIER = 0.05;

    using BinArray = std::vector<float>;

    struct Bins {
        BinArray densities;
        BinArray light;
        pts::SizeInt size;
    };

    struct Environment {
        pts::Point sun;
        pts::BoundingBox boundingBox;

        Environment();
        Environment(pts::Point sun, pts::BoundingBox boundingBox);
    };

    /// Calculates the size of the bin matrix from the bounding box dimensions,
    /// so that every bin is a square.
    pts::SizeInt calculateBinMatrixSize(pts::BoundingBox boundingBox);
}

#endif /* environment_hpp */
