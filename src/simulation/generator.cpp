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

#pragma mark - Private Function Definitions

/// Iterates over every branch of the given tree and creates new branches where appropriate.
Tree treeWithUpdatedBranches(const Tree &tree, const env::Bins &bins, const env::Environment environment);

/// Creates a new child branch for a given parent.
Branch generateChildBranch(const Branch &parent,const pts::Point origin, const TreeParameters &params);


template<typename F>
/// Iterates over the given branch and all of its sub-branches recursively, applying the given
/// lambda to each of them.
void mapBranchRecursively(Branch &branch, const pts::Point origin, const double angle, const env::Bins &bins, const pts::BoundingBox &boundingBox, F mapLambda);

template<typename F>
/// Iterates over the given tree's branches and, for all of the branches in each bin, reduces them
/// into a float using the given lambda.
env::BinArray reduceTreeIntoBins(const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const Tree &tree, F reduceLambda);

double calculateWeight(Branch &branch, double angle, const trees::TreeParameters params, const env::Environment &environment);

template<typename F>
void reduceBranchIntoBinsRecursively(env::BinArray &bins, const pts::SizeInt &matrixSize, const pts::BoundingBox &boundingBox, const pts::Point &point, double angle, const Branch &branch, F reduceLambda);

#pragma mark - Public

Tree gen::iterateTree(const Tree &tree, const env::Bins &bins, const env::Environment &environment) {
    return treeWithUpdatedBranches(tree, bins, environment);
}

env::Bins gen::calculateLightBins(const trees::Tree &tree, const env::Environment &environment) {
    pts::SizeInt matrixSize = env::calculateBinMatrixSize(environment.boundingBox);

    // Calculate the densities for each bin.
    const auto densities = reduceTreeIntoBins(matrixSize, environment.boundingBox, tree, [](float current, float binIndex, Branch branch) {
        return std::min(current + /* branch.radius * */ env::DENSITY_MULTIPLIER, 1.0);
    });

    // Build light matrix from densities.
    env::BinArray lightMatrix = photo::calculateLightMatrixFromDensities(densities, matrixSize, environment);

    env::Bins bins;
    bins.densities = densities;
    bins.light = lightMatrix;
    bins.size = matrixSize;

    return bins;
}

void gen::applyGravitationalForce(trees::Tree &tree, const env::Environment &environment) {
    double totalWeight = calculateWeight(tree.base, 0, tree.params, environment);
}

#pragma mark - Generator Helpers

Tree treeWithUpdatedBranches(const Tree &tree, const env::Bins &bins, const env::Environment environment) {
    trees::Tree newTree = tree;

    mapBranchRecursively(newTree.base, newTree.origin, 0.0, bins, environment.boundingBox, [newTree](Branch &branch, pts::Point origin, double angle, const env::Bins &bins, pts::BoundingBox boundingBox) {
        // Take only the light value from the bin where the branch ends.
        const auto branchEnd = pts::movePoint(origin, angle, branch.length);
        const auto binIndex = pts::worldtoBin(branchEnd, bins.size, boundingBox);
        const auto light = bins.light[binIndex];

        // Grow branch only if it gets sunlight.
        const auto growth = utils::multiplyWithWeight(newTree.params.growthRate, light, 0.8);
        branch.length += growth; // Make each branch longer.
        //branch.radius += sqrt(growth) * newTree.params.branchPossibility;

        // Create a new child branch?
        const auto newBranchPossibilityModifiedByLength = utils::multiplyWithWeight(newTree.params.branchPossibility, std::min(1.0,branch.length/newTree.params.branchoutLength), 1.0);
        const auto newBranchPossibility = utils::multiplyWithWeight(newBranchPossibilityModifiedByLength, light, 1.0);
        if (rnd::randDouble() < newBranchPossibility) {
            Branch newChild = generateChildBranch(branch, origin, newTree.params);
            branch.children.insert(branch.children.end(), newChild);
        }
    });

    return newTree;
}

Branch generateChildBranch(const Branch &parent,const pts::Point origin, const TreeParameters &params) {
    // Make it more probable that a branch appears near the tip of the parent branch rather than at
    // its base.
    const auto position = 1 - pow(rnd::randDouble(), params.branchoutPosition);

    // Choose the side of the new branch, making it more likely that the branch is on a side where
    // there are fewer branches already.
    auto numberOfChildrenWithNegativeAngles = 0;
    for (auto &childBranch : parent.children) {
        if (childBranch.getAngle() < 0) {
            numberOfChildrenWithNegativeAngles++;
        }
    }
    const double branchSideProbability = numberOfChildrenWithNegativeAngles / (double)parent.children.size();
    const auto branchSide = rnd::randBoolWithProbability(branchSideProbability) ? 1 : -1;
    const auto angle = branchSide * M_PI * rnd::randDoubleWithNormDistr(params.branchoutAngleMean, params.branchoutAngleStdDeviation);

    const auto length = 0;
    const auto thickness = 1;

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
    const auto newAngle = angle + branch.getAngle();

    // Get bin indices for the current branch.
    auto branchEnd = pts::movePoint(point, newAngle, branch.length);
    auto binIndices = photo::binIndicesForLine(point, branchEnd, matrixSize, boundingBox);

    // Apply lambda to any bin the branch passed through.
    for (auto binIndex : binIndices) {
        bins[binIndex] = reduceLambda(bins[binIndex], binIndex, branch);
    }

    // Recursively go through children.
    for (auto child : branch.children) {
        auto childOrigin = pts::movePoint(point, newAngle, branch.length * child.position);
        reduceBranchIntoBinsRecursively(bins, matrixSize, boundingBox, childOrigin, newAngle, child, reduceLambda);
    }
}

#pragma mark - Gravity Helpers

double calculateWeight(Branch &branch, double angle, const trees::TreeParameters params, const env::Environment &environment) {

    // calculate the weight of this branch
    double branchWeight = branch.radius * branch.radius * M_PI * branch.length * environment.gravityForce;

    // return weight of empty branch
    if (branch.children.empty()) {
        return branchWeight;
    }

    const auto newAngle = angle + branch.angle;

    //calculate gravitational weigth and force of children pulling down the branch
    double childrenWeightSum = 0;
    double totalForce = 0;
    for (auto &child : branch.children) {
        double weight = calculateWeight(child, newAngle, params, environment);
        childrenWeightSum += weight;

        if (weight > 0) {
            // compute force which pulls down on the branch by his children
            pts::Point gravityForce = environment.gravityVector * weight;

            // create vector that stands orthagonal on the branch
            pts::Point leverForce = pts::createVectorFromAngle(newAngle - M_PI_2);

            // calculate the part of the gravitational force, that pulls orthagonal on the branch
            double force = gravityForce.dot(leverForce) / gravityForce.length();
            totalForce += child.position * force;
        }
    }

    //calculate how much the branch is pulled down
    branch.gravityDelta = totalForce / ( params.springConstant * branch.radius );

    return childrenWeightSum + branchWeight;
}

#pragma mark - Helpers

template<typename F>
void mapBranchRecursively(Branch &branch, const pts::Point origin, const double angle, const env::Bins &bins, const pts::BoundingBox &boundingBox, F mapLambda) {
    const auto currentBranchAngle = angle + branch.getAngle();

    // Map all of the branch's children (passing along the mapping function; when this recursion
    // reaches the leaves, it will apply the mapping function there).
    for (auto &childBranch : branch.children) {
        const auto childOrigin = pts::movePoint(origin, currentBranchAngle, branch.length * childBranch.position);
        mapBranchRecursively(childBranch, childOrigin, currentBranchAngle, bins, boundingBox, mapLambda);
    }

    // Apply the mapping function to the current branch.
    mapLambda(branch, origin, currentBranchAngle, bins, boundingBox);
}
