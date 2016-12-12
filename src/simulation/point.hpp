//
//  point.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#ifndef point_hpp
#define point_hpp

#include <stdio.h>

namespace pts {
    /// A point in space.
    struct Point {
        double x, y;
    };

    /// A size.
    struct Size {
        double width, height;
    };

    /// A bounding box.
    struct BoundingBox {
        Point origin;
        Size size;
    };

    /// Returns the given point moved according to the given angle and length.
    Point movePoint(Point point, double angle, double length);

    /// Returns the lower left coordinate of the corresponding bin
    Point binToWorld(int bin, int columns, int rows, BoundingBox boundingBox);

    /// Returns the index of a bin for the coordinate
    int worldtoBin(Point p, int columns, int rows, BoundingBox boundingBox);
}

#endif /* point_hpp */
