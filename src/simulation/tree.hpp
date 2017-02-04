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

        Branch(double position, double angle, double length, double thickness);
        Branch();
    };

    /// Defines the rules for how a tree grows.
    struct TreeParameters {
        double branchoutAngleMean;
        double branchoutAngleVariance;

        double branchPossibility;

        double growthRate;

        TreeParameters(double branchoutAngleMean, double branchoutAngleVariance, double branchPossibility, double growthRate);
        TreeParameters();
    };

    /// Represents a tree, with the root branch (base/stem/trunk) and an origin point.
    struct Tree {
        pts::Point origin;

        Branch base;
        TreeParameters params;

        /// Returns a tree embryo.
        Tree(pts::Point origin, TreeParameters params);
        Tree();
    };
}

#endif /* tree_hpp */
