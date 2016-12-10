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

/// Progresses the given tree one time unit, returning the new tree.
trees::Tree iterateTree(trees::Tree tree, trees::Point sun);

#endif /* generator_hpp */
