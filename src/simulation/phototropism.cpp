//
//  phototropism.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include <math.h>
#include <string>
#include <vector>

#include "phototropism.hpp"
#include "utils.hpp"

std::vector<int> photo::binIndicesForLine(pts::Point origin, pts::Point destination, pts::SizeInt matrixSize, pts::BoundingBox boundingBox) {

    int bin1 = pts::worldtoBin(origin, matrixSize, boundingBox);
    int bin2 = pts::worldtoBin(destination, matrixSize, boundingBox);

    return binIndicesForLine(bin1, bin2, matrixSize);
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
    // Calculate slope.
    int dx = abs(p2.x - p1.x);
    int dy = -abs(p2.y - p1.y);

    int sgnx = (p2.x > p1.x) ? 1 : -1;
    int sgny = (p2.y > p1.y) ? 1 : -1;

    // Starting error.
    int error = dx+dy;

    // Add start point.
    pts::PointInt currentPosition = p1;
    points.push_back(currentPosition);

    while (currentPosition.x != p2.x || currentPosition.y != p2.y) {
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

env::BinArray photo::combineBins(const env::BinArray &bins1, const env::BinArray &bins2) {
    if (bins1.size() != bins2.size()) {
        throw std::invalid_argument("arrays must have the same size");
    }

    env::BinArray result(bins1.size(), 0.0);
    for (int i = 0; i < result.size(); i++) {
        result[i] = bins1[i] + bins2[i];
    }

    return result;
}
