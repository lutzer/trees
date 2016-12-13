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
#include <vector>

#include "point.hpp"

namespace photo {
    using BinArray = std::vector<float>;

    struct LightBins {
        BinArray densities;
        BinArray light;
        pts::SizeInt size;
    };

    /// calculates the size of the bin matrix from the boundingBox dimensions, so that every bin is a square
    pts::SizeInt calculateBinMatrixSize(pts::BoundingBox boundingBox);

    /// Returns the indices for a line between the given points in the given bounding area.
    std::vector<int> binIndicesForLine(pts::Point origin, pts::Point destination, pts::SizeInt matrixSize, pts::BoundingBox boundingBox);
    std::vector<int> binIndicesForLine(int bin1, int bin2, pts::SizeInt matrixSize);
    
    /// Returns coordinates of the fields a line touches within a matrix. The function uses Bresenham algorithm
    std::vector<pts::PointInt> pointsForLine(pts::PointInt p1, pts::PointInt p2);

    /// Adds the contents of the two given bin arrays together and returns the resulting bin array.
    BinArray combineBins(BinArray bins1, BinArray bins2);

}

#endif /* phototropism_hpp */
