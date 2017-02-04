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

bool rnd::randBool() {
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(engine) == 0;
}

double rnd::randDouble() {
    return rnd::randDouble(0, 1);
}

double rnd::randDouble(double minBound, double maxBound) {
    static std::uniform_real_distribution<double> distribution(minBound, maxBound);
    return distribution(engine);
}

double rnd::randDoubleWithNormDistr(double mean, double standardDeviation) {
    static std::normal_distribution<double> distribution(mean, standardDeviation);
    return distribution(engine);
}
