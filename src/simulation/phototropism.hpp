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

    BinArray combineBins(BinArray bins1, BinArray bins2);

    BinArray normalized(BinArray bins);
}

#endif /* phototropism_hpp */
