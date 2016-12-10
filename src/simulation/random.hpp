//
//  random.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#ifndef random_hpp
#define random_hpp

#include <stdio.h>

static unsigned seed = 0;

/// Returns a random bool.
bool randBool();

/// Returns a random double between 0.0 and 1.0.
double randDouble();

/// Returns a random double within the given bounding values.
double randDouble(double minBound, double maxBound);

#endif /* random_hpp */
