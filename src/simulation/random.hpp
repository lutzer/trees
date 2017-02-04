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

namespace rnd {
    /// Returns a random bool.
    bool randBool();

    /// Returns a random bool according to a Bernoulli distribution, with a probability between 0.0
    /// and 1.0, where 1.0 will always return true and 0.0 will always return false.
    bool randBoolWithProbability(double probability);

    /// Returns a random double between 0.0 and 1.0.
    double randDouble();

    /// Returns a random double within the given bounding values.
    double randDouble(double minBound, double maxBound);

    /// Returns a normally distributed random double with a given mean and standard deviation.
    double randDoubleWithNormDistr(double mean, double standardDeviation);
}

#endif /* random_hpp */
