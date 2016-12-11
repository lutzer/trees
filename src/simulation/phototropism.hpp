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

namespace photo {
    static const int binsPerAxis = 16;

    using BinArray = float[binsPerAxis * binsPerAxis];

    struct LightBins {
        BinArray densities;
        BinArray light;
    };
}

#endif /* phototropism_hpp */
