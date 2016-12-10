//
//  generator.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "generator.hpp"

#include <math.h>

#include "random.hpp"

using namespace trees;

static const double BRANCHOUT_ANGLE_VARIATION = 0.05;
static const double GROWTH_RATE = 0.1;

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(Tree tree);
double brancheoutAngle(double oldAngle, double variation);

template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
Branch mapBranchRecursively(Branch branch, F lambda);

#pragma mark - Public

Tree iterateTree(Tree tree, pts::Point sun) {
    return treeWithUpdatedBranches(tree);
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(Tree tree) {
    auto newTree = tree;

    newTree.base = mapBranchRecursively(newTree.base, [](Branch branch) {
        auto newBranch = branch;
        newBranch.length = branch.length + GROWTH_RATE; // Make each branch longer.

        // Create a new child branch?
        if (randDouble() < 0.1) {
            Branch newChild = {};
            newChild.position = randDouble();
            newChild.angle = brancheoutAngle(branch.angle,BRANCHOUT_ANGLE_VARIATION);
            newChild.length = 0;
            newChild.thickness = 0;
            newBranch.children.insert(newBranch.children.end(), newChild);
        }

        return newBranch;
    });

    return newTree;
}

#pragma mark - Helpers

double brancheoutAngle(double oldAngle, double variation) {
    //return (randDouble(0, M_PI) - M_PI_2) * variation;
    return 0.1;
}

template<typename F>
Branch mapBranchRecursively(Branch branch, F lambda) {
    // If the given branch has no children, just return the branch mapped.
    if (branch.children.size() == 0) {
        return lambda(branch);
    }

    // Otherwise, return the branch with all of its children mapped.
    auto children = branch.children;
    vector<Branch> mappedChildren;
    std::transform(children.begin(), children.end(), std::back_inserter(mappedChildren), [lambda](Branch branch) {
        return mapBranchRecursively(branch, lambda);
    });

    auto newBranch = branch;
    newBranch.children = mappedChildren;
    auto mappedBranch = lambda(newBranch);

    return mappedBranch;
}
