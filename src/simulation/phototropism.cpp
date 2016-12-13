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

pts::SizeInt photo::calculateBinMatrixSize(pts::BoundingBox boundingBox) {
    return {32, 40};
}

std::vector<int> photo::binIndicesForLine(pts::Point origin, pts::Point destination, pts::SizeInt matrixSize, pts::BoundingBox boundingBox) {
    const auto originBinIndex = pts::worldtoBin(origin, matrixSize, boundingBox);
    const auto destinationBinIndex = pts::worldtoBin(destination, matrixSize, boundingBox);

    const auto rowLength = matrixSize.columns;
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

    if (bins1.size() != bins2.size())
        throw std::invalid_argument( "arrays must have the same size" );

    photo::BinArray result(bins1.size(),0.0);
    for (int i = 0; i < result.size(); i++) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}
