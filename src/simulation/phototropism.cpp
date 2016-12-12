//
//  phototropism.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "phototropism.hpp"

#include <math.h>
#include <complex>

int photo::binIndexForPoint(pts::Point point, pts::BoundingBox boundingBox) {
    const auto binSizeX = boundingBox.size.width / (double)photo::binsPerAxis;
    const auto binSizeY = boundingBox.size.height / (double)photo::binsPerAxis;
    const auto x = (int) floor((point.x - boundingBox.origin.x) / binSizeX);
    const auto y = (int) floor((point.y - boundingBox.origin.y) / binSizeY);

    return x + y * photo::binsPerAxis;
}

std::vector<int> photo::binIndicesForLine(pts::Point origin, pts::Point destination, pts::BoundingBox boundingBox) {
    const auto originBinIndex = photo::binIndexForPoint(origin, boundingBox);
    const auto destinationBinIndex = photo::binIndexForPoint(destination, boundingBox);

    const auto rowLength = photo::binsPerAxis;
    const auto destinationX = destinationBinIndex % rowLength;
    const auto destinationY = destinationBinIndex / rowLength;

    std::vector<int> result;
    result.push_back(originBinIndex);
    auto currentBinIndex = originBinIndex;

    while (currentBinIndex != destinationBinIndex) {
        const auto deltaX = destinationX - currentBinIndex % rowLength;
        const auto deltaY = destinationY - currentBinIndex / rowLength;

        if (abs(deltaX) > abs(deltaY)) {
            currentBinIndex = (deltaX > 0 ? currentBinIndex + 1 : currentBinIndex - 1);
        } else {
            currentBinIndex = (deltaY > 0 ? currentBinIndex + rowLength : currentBinIndex - rowLength);
        }
        result.push_back(currentBinIndex);
    }

    return result;
}

photo::BinArray photo::combineBins(photo::BinArray bins1, photo::BinArray bins2) {
    photo::BinArray result;
    result.fill(0.0);

    for (int i = 0; i < result.size(); i++) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}
