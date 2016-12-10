//
//  random.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "random.hpp"

#include <random>

static std::default_random_engine engine(seed);

#pragma mark - Public

bool randBool() {
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(engine) == 0;
}

double randDouble() {
    return randDouble(0, 1);
}

double randDouble(double minBound, double maxBound) {
    static std::uniform_real_distribution<double> distribution(minBound, maxBound);
    return distribution(engine);
}
