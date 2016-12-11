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

/// Progresses the given tree one time unit, returning the new tree.
trees::Tree iterateTree(trees::Tree tree, pts::Point sun);

photo::LightBins lightBinsFromTree(trees::Tree tree, pts::Point sun, pts::BoundingBox boundingBox);

#endif /* generator_hpp */
