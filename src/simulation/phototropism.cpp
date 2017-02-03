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
#include <utils.hpp>

static const float BIN_WIDTH = 2;

pts::SizeInt photo::calculateBinMatrixSize(pts::BoundingBox boundingBox) {

    if (fmod(boundingBox.size.width, BIN_WIDTH) > 0 || fmod(boundingBox.size.height, BIN_WIDTH) > 0) {
        std::string message = "bounding box sizes must be a multiple of " + std::to_string(BIN_WIDTH);
        throw std::invalid_argument(message);
    }

    int columns = boundingBox.size.width / BIN_WIDTH;
    int rows = boundingBox.size.height / BIN_WIDTH;

    return {columns, rows};
}

std::vector<int> photo::binIndicesForLine(pts::Point origin, pts::Point destination, pts::SizeInt matrixSize, pts::BoundingBox boundingBox) {

    int bin1 = pts::worldtoBin(origin, matrixSize, boundingBox);
    int bin2 = pts::worldtoBin(destination, matrixSize, boundingBox);

    return binIndicesForLine(bin1,bin2,matrixSize);
}

std::vector<int> photo::binIndicesForLine(int bin1, int bin2, pts::SizeInt matrixSize) {

    pts::PointInt p1 = {bin1 % matrixSize.columns, bin1 / matrixSize.columns};
    pts::PointInt p2 = {bin2 % matrixSize.columns, bin2 / matrixSize.columns};

    auto points = pointsForLine(p1, p2);

    std::vector<int> result;
    for (auto point : points) {
        result.push_back(point.x + point.y * matrixSize.columns);
    }

    return result;

}

std::vector<pts::PointInt> photo::pointsForLine(pts::PointInt p1, pts::PointInt p2) {
    std::vector<pts::PointInt> points;

    // calculate slope
    int dx = abs(p2.x - p1.x);
    int dy = -abs(p2.y - p1.y);

    int sgnx = p2.x > p1.x ? 1 : -1;
    int sgny = p2.y > p1.y ? 1 : -1;

    // starting error
    int error = dx+dy;

    // add start point
    pts::PointInt currentPosition = p1;
    points.push_back(currentPosition);

    while ( currentPosition.x != p2.x || currentPosition.y != p2.y) {

        int err2 = 2 * error;

        if (err2 > dy) {
            currentPosition.x += sgnx;
            error += dy;
        }
        if (err2 < dx) {
            currentPosition.y += sgny;
            error += dx;
        }
        points.push_back(currentPosition);
    }
    
    return points;
}

photo::BinArray photo::combineBins(const photo::BinArray &bins1, const photo::BinArray &bins2) {

    if (bins1.size() != bins2.size())
        throw std::invalid_argument( "arrays must have the same size" );

    photo::BinArray result(bins1.size(),0.0);
    for (int i = 0; i < result.size(); i++) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}
