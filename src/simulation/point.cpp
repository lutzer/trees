//
//  point.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "point.hpp"

#include <math.h>
#include <utils.hpp>

pts::Point pts::movePoint(pts::Point point, double angle, double length) {
    return { point.x + cos(angle) * length, point.y + sin(angle) * length };
}

pts::Point pts::binToWorld(int bin, pts::SizeInt matrixSize, pts::BoundingBox boundingBox) {

    const auto binSizeX = boundingBox.size.width / matrixSize.columns;
    const auto binSizeY = boundingBox.size.height / matrixSize.rows;

    double x = boundingBox.origin.x + (bin % matrixSize.columns) * binSizeX;
    double y = boundingBox.origin.y + (bin / matrixSize.columns) * binSizeY;

    return {x,y};
}

int pts::worldtoBin(pts::Point point, pts::SizeInt matrixSize, pts::BoundingBox boundingBox) {

    const auto binSizeX = boundingBox.size.width / matrixSize.columns;
    const auto binSizeY = boundingBox.size.height / matrixSize.rows;

    int x = (int) ((point.x - boundingBox.origin.x) / binSizeX);
    int y = (int) ((point.y - boundingBox.origin.y) / binSizeY);

    x = utils::constrainRange(x, 0, matrixSize.columns - 1);
    y = utils::constrainRange(y, 0, matrixSize.rows - 1 );

    return x + y * matrixSize.columns;
}
