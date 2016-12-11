//
//  phototropism.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "phototropism.hpp"

int binIndexForPoint(pts::Point point, pts::BoundingBox boundingBox) {
    auto binSizeX = boundingBox.size.width / (double)photo::binsPerAxis;
    auto binSizeY = boundingBox.size.height / (double)photo::binsPerAxis;
    auto x = (int)(point.x / binSizeX);
    auto y = (int)(point.y / binSizeY);

    return x + y * photo::binsPerAxis;
}

photo::BinArray combineBins(photo::BinArray bins1, photo::BinArray bins2) {
    const int binCount = photo::binsPerAxis * photo::binsPerAxis;
    photo::BinArray result;
    result.fill(0.0);

    for (int i = 0; i++; i < binCount) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}


photo::BinArray normalized(photo::BinArray bins) {
    // TODO: Implement this.
    return bins;
}
