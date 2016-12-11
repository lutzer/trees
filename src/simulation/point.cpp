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
