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

    Point movePoint(Point point, double angle, double length);
}

#endif /* point_hpp */
