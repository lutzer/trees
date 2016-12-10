//
//  random.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "random.hpp"

#include <random>

std::default_random_engine generator();

#pragma mark - Public

bool randBool() {
    std::uniform_int_distribution<int> distribution(0, 1);
    auto gen = generator();

    return distribution(gen) == 0;
}

double randDouble(double minBound, double maxBound) {
    std::uniform_real_distribution<double> distribution(minBound, maxBound);
    auto gen = generator();

    return distribution(gen);
}

#pragma mark - Helpers

std::default_random_engine generator() {
    std::default_random_engine engine(seed);
    return engine;
}
