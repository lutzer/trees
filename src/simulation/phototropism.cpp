//
//  phototropism.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#include "phototropism.hpp"

static const int binsPerAxis = 16;

struct LightBins {
    float densities[binsPerAxis][binsPerAxis];
    float light[binsPerAxis][binsPerAxis];
};
