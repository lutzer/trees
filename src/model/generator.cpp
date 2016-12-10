//
//  generator.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "generator.hpp"

using namespace trees;

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree createBranches(Tree tree);

template<typename F>
/// Iterates over the given branch and all of its subbranches recursively, applying the given lambda
/// to each of them.
Branch mapBranch(Branch branch, F lambda);

#pragma mark - Public

Tree iterateTree(Tree tree, pts::Point sun) {
    return createBranches(tree);
}

#pragma mark - Generator Helpers

Tree createBranches(Tree tree) {
    auto branchMappingFunction = [] (Branch branch) {
        auto newBranch = branch;
        newBranch.length = branch.length + 0.5;

        return newBranch;
    };

    auto newTree = tree;
    auto newBase = mapBranch(newTree.base, branchMappingFunction);
    newTree.base = newBase;

    return newTree;
}

#pragma mark - Helpers

template<typename F>
Branch mapBranch(Branch branch, F lambda) {
    auto newBranch = lambda(branch);
    return newBranch;
}
