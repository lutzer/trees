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

env::BinArray photo::calculateLightMatrixFromDensities(const env::BinArray &densities, const pts::SizeInt &matrixSize, const env::Environment &environment) {

    env::BinArray lightMatrix(matrixSize.columns * matrixSize.rows, 0.0);

    // get indizes of all border fields
    vector<int> borderIndices;
    // add column indices
    for (int i=0; i < matrixSize.columns; i++) {
        borderIndices.push_back(i);
        borderIndices.push_back(i + (matrixSize.rows-1) * matrixSize.columns);
    }
    // add row indices
    for (int i=1; i < matrixSize.rows-1; i++) {
        borderIndices.push_back(i * matrixSize.columns );
        borderIndices.push_back((i+1) * matrixSize.columns -1 );
    }

    int sunBin = pts::worldtoBin(environment.sun, matrixSize, environment.boundingBox);

    // fill the matrix by calculating the lines from the sun to all border fields of the matrix
    for (int i : borderIndices) {

        if (lightMatrix[i] > 0.0)
            continue;

        vector<int> binIndices = photo::binIndicesForLine(sunBin, i, matrixSize);
        float densitySum = 0.0;
        for (int index : binIndices) {
            densitySum += densities[index];
            lightMatrix[index] = std::max(0.0, 1.0 - densitySum);
        }
    }



    return lightMatrix;
}

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
