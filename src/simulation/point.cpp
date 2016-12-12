//
//  point.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "point.hpp"

#include <math.h>

pts::Point pts::movePoint(pts::Point point, double angle, double length) {
    return { point.x + cos(angle) * length, point.y + sin(angle) * length };
}

pts::Point pts::binToWorld(int bin, int columns, int rows, pts::BoundingBox boundingBox) {

    const auto binSizeX = boundingBox.size.width / columns;
    const auto binSizeY = boundingBox.size.height / rows;

    double x = boundingBox.origin.x + (bin % columns) * binSizeX;
    double y = boundingBox.origin.y + (bin / columns) * binSizeY;

    return {x,y};
}

int pts::worldtoBin(pts::Point point, int columns, int rows, pts::BoundingBox boundingBox) {

    const auto binSizeX = boundingBox.size.width / columns;
    const auto binSizeY = boundingBox.size.height / rows;

    const auto x = (int) floor((point.x - boundingBox.origin.x) / binSizeX);
    const auto y = (int) floor((point.y - boundingBox.origin.y) / binSizeY);

    return x + y * columns;
}
