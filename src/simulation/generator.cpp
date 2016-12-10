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

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(Tree tree);

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
        newBranch.length = branch.length + 0.5; // Make each branch longer.

        // Create a new child branch?
        if (randBool()) {
            Branch newChild = {};
            newChild.position = randDouble();
            newChild.angle = randDouble(0, M_PI);
            newChild.length = 0;
            newChild.thickness = 0;
            newBranch.children.insert(newBranch.children.end(), newChild);
        }

        return newBranch;
    });

    return newTree;
}

#pragma mark - Helpers

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

    Branch mappedBranch = lambda(branch);
    mappedBranch.children = mappedChildren;

    return mappedBranch;
}
