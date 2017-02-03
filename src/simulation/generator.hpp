//
//  generator.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#ifndef generator_hpp
#define generator_hpp

#include <stdio.h>

#include "tree.hpp"
#include "phototropism.hpp"

namespace gen {

    /// Progresses the given tree one time unit, returning the new tree.
    trees::Tree iterateTree(trees::Tree &tree, photo::LightBins &bins, pts::BoundingBox boundingBox);

    /// Calculates the leafage density and available light for each bin in the given bounding box.
    photo::LightBins calculateLightBins(trees::Tree &tree, pts::Point sun, pts::BoundingBox boundingBox);
}

#endif /* generator_hpp */
