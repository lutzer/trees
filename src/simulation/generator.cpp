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
#include <algorithm>
#include <map>
#include <vector>

#include "random.hpp"
#include "utils.hpp"

using namespace trees;

static const double DENSITY_MULTIPLIER = 0.1; // Multiplied with branch thickness to sum up density values.

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(const Tree &tree, const photo::LightBins &bins, pts::BoundingBox boundingBox);

/// Creates a new child branch for a given parent.
Branch generateChildBranch(const Branch &parent, const TreeParameters params);

template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
Branch mapBranchRecursively(const Branch &branch, const pts::Point origin, const double angle, photo::LightBins bins, pts::BoundingBox boundingBox, F mapLambda);

template<typename F>
/// Iterates over the given tree's branches and, for all of the branches in each bin, reduces them
/// into a float using the given lambda.
photo::BinArray reduceTreeIntoBins(pts::SizeInt matrixSize, pts::BoundingBox boundingBox, Tree tree, F reduceLambda);

template<typename F>
void reduceBranchIntoBinsRecursively(photo::BinArray &bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, const Branch &branch, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(const Tree &tree, const photo::LightBins &bins, pts::BoundingBox boundingBox) {
    return treeWithUpdatedBranches(tree, bins, boundingBox);
}

photo::LightBins gen::calculateLightBins(const Tree &tree, pts::Point sun, pts::BoundingBox boundingBox) {
    pts::SizeInt matrixSize = photo::calculateBinMatrixSize(boundingBox);

    // Calculate the densities for each bin.
    auto densities = reduceTreeIntoBins(matrixSize, boundingBox, tree, [](float current, float binIndex, Branch branch) {
        return std::min(current + branch.thickness * DENSITY_MULTIPLIER,1.0);
    });

    // Normalize the densities.
    //utils::normalize(densities);

    // Build light matrix from densities.
    photo::BinArray lightMatrix(matrixSize.columns * matrixSize.rows, 0.0);
    for (int i = 0; i < lightMatrix.size(); i++) {
        int sunBin = pts::worldtoBin(sun, matrixSize, boundingBox);

        vector<int> binIndices = photo::binIndicesForLine(i, sunBin, matrixSize);
        float densitySum = 0.0;
        for (int index : binIndices) {
            densitySum += densities[index];
        }
        lightMatrix[i] = std::max(0.0, 1.0 - densitySum);
    }
    return { densities, lightMatrix, matrixSize };
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(const Tree &tree, const photo::LightBins &bins, pts::BoundingBox boundingBox) {
    auto newTree = tree;

    newTree.base = mapBranchRecursively(newTree.base, newTree.origin, 0.0, bins, boundingBox, [tree](Branch branch, pts::Point origin, double angle, const photo::LightBins &bins, pts::BoundingBox boundingBox) {
        const auto newAngle = angle + branch.angle;

        // Take only the light value from the bin where the branch ends.
        auto branchEnd = pts::movePoint(origin, newAngle, branch.length);
        int binIndex = pts::worldtoBin(branchEnd, bins.size, boundingBox);
        auto light = bins.light[binIndex];

        auto newBranch = branch;
        // Grow branch only if it gets sunlight.
        newBranch.length = branch.length + tree.params.growthRate * light; // Make each branch longer.
        newBranch.thickness = 1;

        // Create a new child branch?
        if (randDouble() < tree.params.branchPossibility * light) {
            Branch newChild = generateChildBranch(newBranch, tree.params);
            newBranch.children.insert(newBranch.children.end(), newChild);
        }

        return newBranch;
    });

    return newTree;
}

Branch generateChildBranch(const Branch &parent, const TreeParameters params) {
    Branch newChild = {};
    newChild.position = randDouble();
    newChild.angle = randDouble() * params.branchoutAngleVariance * 2 - params.branchoutAngleVariance;
    newChild.length = 0;
    newChild.thickness = 1;

    return newChild;
}

#pragma mark - Binning Helpers

template<typename F>
photo::BinArray reduceTreeIntoBins(pts::SizeInt matrixSize, pts::BoundingBox boundingBox, Tree tree, F reduceLambda) {
    photo::BinArray bins(matrixSize.columns * matrixSize.rows, 0.0);
    reduceBranchIntoBinsRecursively(bins, matrixSize, boundingBox, tree.origin, 0.0, tree.base, reduceLambda);
    return bins;
}

template<typename F>
void reduceBranchIntoBinsRecursively(photo::BinArray &bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, const Branch &branch, F reduceLambda) {
    const auto newAngle = angle + branch.angle;

    // Get bin indices for the current branch.
    auto branchEnd = pts::movePoint(point, newAngle, branch.length);
    auto binIndices = photo::binIndicesForLine(point, branchEnd, matrixSize, boundingBox);

    // apply lambda to any bin the branch passed through
    for (auto binIndex : binIndices) {
        bins[binIndex] = reduceLambda(bins[binIndex], binIndex, branch);
    }

    // recursivly go through children
    for (trees::Branch child : branch.children) {
        auto childOrigin = pts::movePoint(point, newAngle, branch.length * child.position);
        reduceBranchIntoBinsRecursively(bins, matrixSize, boundingBox, childOrigin, newAngle, child, reduceLambda);
    }
}

#pragma mark - Helpers

template<typename F>
Branch mapBranchRecursively(const Branch &branch, const pts::Point origin, const double angle, photo::LightBins bins, pts::BoundingBox boundingBox, F mapLambda) {
    // If the given branch has no children, just return the branch mapped.
    if (branch.children.size() == 0) {
        return mapLambda(branch, origin, angle, bins, boundingBox);
    }

    // Otherwise, return the branch with all of its children mapped.
    auto children = branch.children;
    std::vector<Branch> mappedChildren = utils::map<Branch>(children,  [mapLambda, origin, angle, bins, boundingBox](Branch child) {
        return mapBranchRecursively(child, origin, angle, bins, boundingBox, mapLambda);
    });

    auto newBranch = branch;
    newBranch.children = mappedChildren;
    auto mappedBranch = mapLambda(newBranch, origin, angle, bins, boundingBox);

    return mappedBranch;
}
