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

    /// A point in a matrix
    struct PointInt {
        int x, y;
    };

    /// A size.
    struct Size {
        double width, height;
    };

    /// Size of a matrix
    struct SizeInt {
        int columns, rows;
    };

    /// A bounding box.
    struct BoundingBox {
        Point origin;
        Size size;
    };

    /// Returns the given point moved according to the given angle and length.
    Point movePoint(Point point, double angle, double length);

    /// Returns the center coordinate of the corresponding bin
    Point binToWorld(int bin, SizeInt matrixSize, BoundingBox boundingBox);

    /// Returns the index of a bin for the coordinate
    int worldtoBin(Point p, SizeInt matrixSize, BoundingBox boundingBox);
}

#endif /* point_hpp */
