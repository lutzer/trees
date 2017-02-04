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

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(const Tree &tree, const env::Bins &bins, const env::Environment environment);

/// Creates a new child branch for a given parent.
Branch generateChildBranch(const Branch &parent, const TreeParameters &params);

template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
void mapBranchRecursively(Branch branch, const pts::Point origin, const double angle, const env::Bins &bins, const pts::BoundingBox &boundingBox, F mapLambda);

template<typename F>
/// Iterates over the given tree's branches and, for all of the branches in each bin, reduces them
/// into a float using the given lambda.
env::BinArray reduceTreeIntoBins(const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const Tree &tree, F reduceLambda);

template<typename F>
void reduceBranchIntoBinsRecursively(env::BinArray &bins, const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const pts::Point &point, double angle, const Branch &branch, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(const Tree &tree, const env::Bins &bins, const env::Environment &environment) {
    return treeWithUpdatedBranches(tree, bins, environment);
}

env::Bins gen::calculateLightBins(const trees::Tree &tree, const env::Environment &environment) {
    
    pts::SizeInt matrixSize = env::calculateBinMatrixSize(environment.boundingBox);

    // Calculate the densities for each bin.
    auto densities = reduceTreeIntoBins(matrixSize, environment.boundingBox, tree, [](float current, float binIndex, Branch branch) {
        return std::min(current + branch.thickness * env::DENSITY_MULTIPLIER,1.0);
    });

    // Build light matrix from densities.
    env::BinArray lightMatrix(matrixSize.columns * matrixSize.rows, 0.0);
    for (int i = 0; i < lightMatrix.size(); i++) {
        int sunBin = pts::worldtoBin(environment.sun, matrixSize, environment.boundingBox);

        vector<int> binIndices = photo::binIndicesForLine(i, sunBin, matrixSize);
        float densitySum = 0.0;
        for (int index : binIndices) {
            densitySum += densities[index];
        }
        lightMatrix[i] = std::max(0.0, 1.0 - densitySum);
    }

    env::Bins bins;
    bins.densities = densities;
    bins.light = lightMatrix;
    bins.size = matrixSize;

    return bins;
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(const Tree &tree, const env::Bins &bins, const env::Environment environment) {

    trees::Tree newTree = tree;

    mapBranchRecursively(newTree.base, newTree.origin, 0.0, bins, environment.boundingBox, [newTree](Branch branch, pts::Point origin, double angle, const env::Bins &bins, pts::BoundingBox boundingBox) {
        const auto newAngle = angle + branch.angle;

        // Take only the light value from the bin where the branch ends.
        auto branchEnd = pts::movePoint(origin, newAngle, branch.length);
        int binIndex = pts::worldtoBin(branchEnd, bins.size, boundingBox);
        auto light = bins.light[binIndex];

        // Grow branch only if it gets sunlight.
        branch.length = branch.length + newTree.params.growthRate * light; // Make each branch longer.
        //branch.thickness = 1;

        // Create a new child branch?
        if (randDouble() < newTree.params.branchPossibility) {
            Branch newChild = generateChildBranch(branch, newTree.params);
            branch.children.insert(branch.children.end(), newChild);
        }
    });

    return newTree;
}

Branch generateChildBranch(const Branch &parent, const TreeParameters &params) {
    auto position = randDouble();
    auto angle = randDouble() * params.branchoutAngleVariance * 2 - params.branchoutAngleVariance;
    auto length = 0;
    auto thickness = 1;

    return Branch(position, angle, length, thickness);
}

#pragma mark - Binning Helpers

template<typename F>
env::BinArray reduceTreeIntoBins(const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const Tree &tree, F reduceLambda) {
    env::BinArray bins(matrixSize.columns * matrixSize.rows, 0.0);
    reduceBranchIntoBinsRecursively(bins, matrixSize, boundingBox, tree.origin, 0.0, tree.base, reduceLambda);
    return bins;
}

template<typename F>
void reduceBranchIntoBinsRecursively(env::BinArray &bins, const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const pts::Point &point, double angle, const Branch &branch, F reduceLambda) {
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
void mapBranchRecursively(Branch branch, const pts::Point origin, const double angle, const env::Bins &bins, const pts::BoundingBox &boundingBox, F mapLambda) {
    // If the given branch has no children, just return the branch mapped.
    if (branch.children.size() == 0) {
        return mapLambda(branch, origin, angle, bins, boundingBox);
    }

    // Otherwise, return the branch with all of its children mapped.
    auto children = branch.children;
    for (auto childBranch : children) {
        mapBranchRecursively(childBranch, origin, angle, bins, boundingBox, mapLambda);
    }

    // apply function to children
    mapLambda(branch, origin, angle, bins, boundingBox);

}
