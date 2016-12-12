//
//  phototropism.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#ifndef phototropism_hpp
#define phototropism_hpp

#include <stdio.h>
#include <array>
#include <vector>

#include "point.hpp"

namespace photo {
    static const int binsPerAxis = 16;

    using BinArray = std::array<float, binsPerAxis * binsPerAxis>;

    struct LightBins {
        BinArray densities;
        BinArray light;
    };

    /// Returns the index for a given point in the given bounding area.
    int binIndexForPoint(pts::Point point, pts::BoundingBox boundingBox);

    /// Returns the indices for a line between the given points in the given bounding area.
    std::vector<int> binIndicesForLine(pts::Point origin, pts::Point destination, pts::BoundingBox boundingBox);

    /// Adds the contents of the two given bin arrays together and returns the resulting bin array.
    BinArray combineBins(BinArray bins1, BinArray bins2);
}

#endif /* phototropism_hpp */
