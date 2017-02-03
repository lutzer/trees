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
static const double GROWTH_RATE = 0.2;
static const double BRANCH_POSSIBLITY = 0.1;
static const double DENSITY_MULTIPLIER = 0.1; // multiplied with branch thickness to sum up density values
static const int MAX_BRANCHES = 50; // maximal branchout number
static const int MAX_DEPTH = 20; // maximal depth of a tree

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(Tree &tree, photo::LightBins &bins, pts::BoundingBox boundingBox);

/// Creates a new child branch for a given parent.
Branch generateChildBranch(Branch &parent);

template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
Branch mapBranchRecursively(Branch &branch, const pts::Point origin, const double angle, photo::LightBins bins, pts::BoundingBox boundingBox, F mapLambda);

template<typename F>
/// Iterates over the given tree's branches and, for all of the branches in each bin, reduces them
/// into a float using the given lambda.
photo::BinArray reduceTreeIntoBins(pts::SizeInt matrixSize, pts::BoundingBox boundingBox, Tree tree, F reduceLambda);

template<typename F>
void reduceBranchIntoBinsRecursively(photo::BinArray &bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch &branch, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(Tree &tree, photo::LightBins &bins, pts::BoundingBox boundingBox) {
    return treeWithUpdatedBranches(tree, bins, boundingBox);
}

photo::LightBins gen::calculateLightBins(Tree &tree, pts::Point sun, pts::BoundingBox boundingBox) {
    pts::SizeInt matrixSize = photo::calculateBinMatrixSize(boundingBox);

    // calculate densities for each bin
    auto densities = reduceTreeIntoBins(matrixSize, boundingBox, tree, [](float current, float binIndex, Branch branch) {
        return std::min(current + branch.thickness * DENSITY_MULTIPLIER,1.0);
    });

    // normalize denseties
    //utils::normalize(densities);

    // build light matrix from densities
    photo::BinArray lightMatrix(matrixSize.columns * matrixSize.rows, 0.0);
    for (int i = 0; i < lightMatrix.size(); i++) {

        int sunBin = pts::worldtoBin(sun, matrixSize, boundingBox);

        vector<int> binIndices = photo::binIndicesForLine(i, sunBin, matrixSize);
        binIndices.erase(binIndices.begin()); // remove first element
        float densitySum = 0.0;
        for (int index : binIndices) {
            densitySum += densities[index];
        }
        lightMatrix[i] = std::max(0.0, 1.0 - densitySum);
    }
    return { densities, lightMatrix, matrixSize };
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(Tree &tree, photo::LightBins &bins, pts::BoundingBox boundingBox) {
    auto newTree = tree;

    newTree.base = mapBranchRecursively(newTree.base, newTree.origin, 0.0, bins, boundingBox, [](Branch branch, pts::Point origin, double angle, photo::LightBins &bins, pts::BoundingBox boundingBox) {
        const auto newAngle = angle + branch.angle;

        // take only the light value from the bin where the branch ends
        auto branchEnd = pts::movePoint(origin, newAngle, branch.length);
        int binIndex = pts::worldtoBin(branchEnd, bins.size, boundingBox);
        auto light = bins.light[binIndex];

        auto newBranch = branch;

        // grow branch faster if there is no sunlight
        float growth = GROWTH_RATE / std::max(0.1F,light);
        newBranch.length = branch.length + growth; // Make each branch longer.
        //newBranch.thickness = branch.thickness + sqrt(growth);

        // Create a new child branch, possibility depending on the light
        if (MAX_BRANCHES > newBranch.children.size() && randDouble() < (BRANCH_POSSIBLITY * light)) {

            Branch newChild = generateChildBranch(newBranch);
            newBranch.children.insert(newBranch.children.end(), newChild);
        }

        return newBranch;
    });

    return newTree;
}

Branch generateChildBranch(Branch &parent) {
    Branch newChild = {};
    newChild.position = randDouble();
    newChild.angle = randDouble() * BRANCHOUT_ANGLE_VARIATION * 2 - BRANCHOUT_ANGLE_VARIATION;
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
void reduceBranchIntoBinsRecursively(photo::BinArray &bins, pts::SizeInt matrixSize, pts::BoundingBox boundingBox, pts::Point point, double angle, Branch &branch, F reduceLambda) {

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
Branch mapBranchRecursively(Branch &branch, const pts::Point origin, const double angle, photo::LightBins bins, pts::BoundingBox boundingBox, F mapLambda) {
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
