//
//  generator.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "generator.hpp"

#include <math.h>
#include <array>
#include <numeric>

#include "random.hpp"

using namespace trees;

static const double BRANCHOUT_ANGLE_VARIATION = 0.5;
static const double GROWTH_RATE = 0.25;
static const double BRANCH_POSSIBLITY = 0.1;

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(Tree tree);

/// Creates a new child branch for a given parent.
Branch generateChildBranch(Branch parent);

template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
Branch mapBranchRecursively(Branch branch, F mapLambda);

template<typename F>
/// Iterates over the given tree's branches and, for all of the branches in each bin, reduces them
/// into a float using the given lambda.
photo::BinArray reduceTreeIntoBins(pts::BoundingBox boundingBox, Tree tree, pts::Point sun, F reduceLambda);

template<typename F>
photo::BinArray reduceBranchIntoBinsRecursively(photo::BinArray bins, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch branch, pts::Point sun, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(Tree tree, pts::Point sun) {
    return treeWithUpdatedBranches(tree);
}

photo::LightBins gen::lightBinsFromTree(Tree tree, pts::Point sun, pts::BoundingBox boundingBox) {
    auto densities = reduceTreeIntoBins(boundingBox, tree, sun, [](float current, float binIndex, Branch branch) {
        return current + branch.thickness;
    });
    auto normalizedDensities = photo::normalized(densities);

    photo::BinArray light;
    light.fill(0.0);
    std::transform(normalizedDensities.begin(), normalizedDensities.end(), light.begin(), [sun, boundingBox, normalizedDensities](float density) {
        // TODO: Take into account all the densities between the bin and the sun.
        const auto sunBinIndex = photo::binIndexForPoint(sun, boundingBox);
        return 1.0 - density;
    });

    return { normalizedDensities, light };
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(Tree tree) {
    auto newTree = tree;

    newTree.base = mapBranchRecursively(newTree.base, [](Branch branch) {
        auto newBranch = branch;
        newBranch.length = branch.length + GROWTH_RATE; // Make each branch longer.
        newBranch.thickness = branch.thickness + GROWTH_RATE / (2 * M_PI); // Make each branch thicker.

        // Create a new child branch?
        if (randDouble() < BRANCH_POSSIBLITY) {
            Branch newChild = generateChildBranch(newBranch);
            newBranch.children.insert(newBranch.children.end(), newChild);
        }

        return newBranch;
    });

    return newTree;
}

Branch generateChildBranch(Branch parent) {
    Branch newChild = {};
    newChild.position = randDouble();
    newChild.angle = randDouble() * BRANCHOUT_ANGLE_VARIATION * 2 - BRANCHOUT_ANGLE_VARIATION;
    newChild.length = 0;
    newChild.thickness = 0;

    return newChild;
}

#pragma mark - Binning Helpers

template<typename F>
photo::BinArray reduceTreeIntoBins(pts::BoundingBox boundingBox, Tree tree, pts::Point sun, F reduceLambda) {
    photo::BinArray emptyBins;
    emptyBins.fill(0.0);
    return reduceBranchIntoBinsRecursively(emptyBins, boundingBox, tree.origin, 0.0, tree.base, sun, reduceLambda);
}

template<typename F>
photo::BinArray reduceBranchIntoBinsRecursively(photo::BinArray bins, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch branch, pts::Point sun, F reduceLambda) {
    const auto newAngle = angle + branch.angle;

    // Get bin indices for the current branch.
    auto branchEnd = pts::movePoint(point, newAngle, branch.length);
    auto binIndices = photo::binIndicesForLine(point, branchEnd, boundingBox);

    auto newBins = bins;
    for (auto binIndex : binIndices) {
        newBins[binIndex] = reduceLambda(newBins[binIndex], binIndex, branch);
    }

    // If the given branch has no children, return the bin array with the branch reduced into it.
    if (branch.children.size() == 0) {
        return newBins;
    }

    // Otherwise, return the bin array with all of the branch's children reduced into it.
    std::vector<photo::BinArray> mappedChildren;
    std::transform(branch.children.begin(), branch.children.end(), std::back_inserter(mappedChildren), [bins, boundingBox, point, newAngle, sun, branch, reduceLambda](Branch child) {
        auto newPoint = pts::movePoint(point, newAngle, branch.length * child.position);
        return reduceBranchIntoBinsRecursively(bins, boundingBox, newPoint, newAngle, child, sun, reduceLambda);
    });

    return std::accumulate(mappedChildren.begin(), mappedChildren.end(), newBins, [](photo::BinArray acc, photo::BinArray curr) {
        return photo::combineBins(acc, curr);
    });
}

#pragma mark - Helpers

template<typename F>
Branch mapBranchRecursively(Branch branch, F mapLambda) {
    // If the given branch has no children, just return the branch mapped.
    if (branch.children.size() == 0) {
        return mapLambda(branch);
    }

    // Otherwise, return the branch with all of its children mapped.
    auto children = branch.children;
    std::vector<Branch> mappedChildren;
    std::transform(children.begin(), children.end(), std::back_inserter(mappedChildren), [mapLambda](Branch child) {
        return mapBranchRecursively(child, mapLambda);
    });

    auto newBranch = branch;
    newBranch.children = mappedChildren;
    auto mappedBranch = mapLambda(newBranch);

    return mappedBranch;
}
