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
#include <math.h>

#include "tree.hpp"
#include "phototropism.hpp"
#include "environment.hpp"

namespace gen {

    /// Progresses the given tree one time unit, returning the new tree.
    trees::Tree iterateTree(const trees::Tree &tree, const env::Bins &bins, const env::Environment &environment);

    void applyGravitationalForce(trees::Tree &tree, const env::Environment &environment);

    /// Calculates the leafage density and available light for each bin in the given bounding box.
    env::Bins calculateLightBins(const trees::Tree &tree, const env::Environment &environment);
}

#endif /* generator_hpp */
