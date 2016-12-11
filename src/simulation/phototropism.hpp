//
//  phototropism.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-11.
//
//

#ifndef phototropism_hpp
#define phototropism_hpp

#include <stdio.h>

static const int binsPerAxis = 16;

struct LightBins {
    float densities[binsPerAxis][binsPerAxis];
    float light[binsPerAxis][binsPerAxis];
};

#endif /* phototropism_hpp */
