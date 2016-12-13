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
#include "utils.hpp"

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
photo::BinArray reduceTreeIntoBins(pts::SizeInt matrixSize, pts::BoundingBox boundingBox, Tree tree, F reduceLambda);

template<typename F>
photo::BinArray reduceBranchIntoBinsRecursively(photo::BinArray bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch branch, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(Tree tree, photo::LightBins bins) {
    return treeWithUpdatedBranches(tree);
}

photo::LightBins gen::lightBinsFromTree(Tree tree, pts::Point sun, pts::BoundingBox boundingBox) {

    pts::SizeInt matrixSize = photo::calculateBinMatrixSize(boundingBox);

    // calculate densities for each bin
    auto densities = reduceTreeIntoBins(matrixSize, boundingBox, tree, [](float current, float binIndex, Branch branch) {
        return current + branch.thickness;
    });

    // normalize denseties
    utils::normalize(densities);

    // build light matrix from densities
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
photo::BinArray reduceTreeIntoBins(pts::SizeInt matrixSize, pts::BoundingBox boundingBox, Tree tree, F reduceLambda) {
    photo::BinArray emptyBins(matrixSize.columns * matrixSize.rows, 0.0);
    return reduceBranchIntoBinsRecursively(emptyBins, matrixSize, boundingBox, tree.origin, 0.0, tree.base, reduceLambda);
}

template<typename F>
photo::BinArray reduceBranchIntoBinsRecursively(photo::BinArray bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch branch, F reduceLambda) {
    const auto newAngle = angle + branch.angle;

    // Get bin indices for the current branch.
    auto branchEnd = pts::movePoint(point, newAngle, branch.length);
    auto binIndices = photo::binIndicesForLine(point, branchEnd, matrixSize, boundingBox);

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
    std::transform(branch.children.begin(), branch.children.end(), std::back_inserter(mappedChildren), [bins, matrixSize, boundingBox, point, newAngle, branch, reduceLambda](Branch child) {
        auto newPoint = pts::movePoint(point, newAngle, branch.length * child.position);
        return reduceBranchIntoBinsRecursively(bins, matrixSize, boundingBox, newPoint, newAngle, child, reduceLambda);
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
