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

    /// Defines the rules for how a tree grows.
    struct TreeParameters {
        double branchoutAngleMean;
        double branchoutAngleVariance;

        double growthRate;
        double branchPossibility;
    };

    /// Represents a tree, with the root branch (base/stem/trunk) and an origin point.
    struct Tree {
        Branch base;
        pts::Point origin;
        TreeParameters params;
    };

    /// Returns a tree embryo.
    Tree generateSapling(pts::Point origin);
}  // namespace trees


#endif /* tree_hpp */
