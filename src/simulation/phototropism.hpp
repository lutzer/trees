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
#include "environment.hpp"

namespace photo {

    /// Calculates the light matrix from the density array
    env::BinArray calculateLightMatrixFromDensities(const env::BinArray &densities, const pts::SizeInt &matrixSize, const env::Environment &environment);

    /// Returns the indices for a line between the given points in the given bounding area.
    std::vector<int> binIndicesForLine(pts::Point origin, pts::Point destination, pts::SizeInt matrixSize, pts::BoundingBox boundingBox);
    std::vector<int> binIndicesForLine(int bin1, int bin2, pts::SizeInt matrixSize);

    /// Returns coordinates of the fields a line touches within a matrix. The function uses Bresenham algorithm
    std::vector<pts::PointInt> pointsForLine(pts::PointInt p1, pts::PointInt p2);

    /// Adds the contents of the two given bin arrays together and returns the resulting bin array.
    env::BinArray combineBins(const env::BinArray &bins1,const env::BinArray &bins2);

}

#endif /* phototropism_hpp */
