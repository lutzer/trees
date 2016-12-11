//
//  tree.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#ifndef tree_hpp
#define tree_hpp

#include <stdio.h>
#include <vector>

#include "point.hpp"

namespace trees {
    /// Represents a branch on a tree, including some attributes and child branches.
    struct Branch {
        double position, angle, length, thickness;
        std::vector<Branch> children;
    };

    /// Represents a tree, with the root branch (base/stem/trunk) and an origin point.
    struct Tree {
        Branch base;
        pts::Point origin;
    };

    /// Returns a tree embryo.
    Tree generateSapling();
}


#endif /* tree_hpp */
