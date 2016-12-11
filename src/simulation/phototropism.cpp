//
//  phototropism.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "phototropism.hpp"

#include <math.h>

int photo::binIndexForPoint(pts::Point point, pts::BoundingBox boundingBox) {
    const auto binSizeX = boundingBox.size.width / (double)photo::binsPerAxis;
    const auto binSizeY = boundingBox.size.height / (double)photo::binsPerAxis;
    const auto x = (int) floor((point.x - boundingBox.origin.x) / binSizeX);
    const auto y = (int) floor((point.y - boundingBox.origin.y) / binSizeY);

    return x + y * photo::binsPerAxis;
}

photo::BinArray photo::combineBins(photo::BinArray bins1, photo::BinArray bins2) {
    photo::BinArray result;
    result.fill(0.0);

    for (int i = 0; i < result.size(); i++) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}


photo::BinArray photo::normalized(photo::BinArray bins) {
    // TODO: Implement this.
    return bins;
}
