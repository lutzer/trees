//
//  tree.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "tree.hpp"

#include <math.h>
#include <iostream>

trees::Tree trees::generateSapling(pts::Point origin) {
    return { { 0, M_PI_2, 1, 1, {} }, origin };
}
