//
//  tree.cpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#include "tree.hpp"

#include <math.h>
#include <iostream>

#include "environment.hpp"

using namespace trees;

Branch::Branch(double position, double angle, double length, double radius) {
    this->position = position;
    this->angle = angle;
    this->length = length;
    this->radius = radius;
}

Branch::Branch() {
    Branch(0, M_PI_2, 0, 1);
}

double Branch::getAngle() const {
    return this->angle + this->gravityDelta;
}

TreeParameters::TreeParameters(double branchoutAngleMean, double branchoutAngleStdDeviation, double branchPossibility, double branchoutLength, double growthRate) {
    this->branchoutAngleMean = branchoutAngleMean;
    this->branchoutAngleStdDeviation = branchoutAngleStdDeviation;
    this->growthRate = growthRate;
    this->branchPossibility = branchPossibility;
    this->branchoutLength = branchoutLength;
    this->springConstant = 10;
}

TreeParameters::TreeParameters() {
    TreeParameters(1, 1, 1, 1, 1);
}

Tree::Tree(pts::Point origin, double trunkAngle, TreeParameters params) {
    this->origin = origin;
    this->params = params;
    this->base = Branch(0, trunkAngle, 1, 1);
}

Tree::Tree() {
    Tree({0,0}, M_PI_2, TreeParameters());
}

void iterateBranches(const Branch &branch, int &numberOfBranches, float &volume, int &depth, int currentDepth) {

    depth = std::max(depth,currentDepth);
    volume += branch.radius * branch.radius * M_PI * branch.length;
    numberOfBranches += branch.children.size();

    for( auto child : branch.children) {
        iterateBranches(child, numberOfBranches, volume, depth, currentDepth + 1);
    }
}

std::string Tree::toString() {

    int numberOfBranches = 1;
    int depth = 0;
    float volume = 0;

    // calculates depth, number of branches and biomass
    iterateBranches(this->base, numberOfBranches, volume, depth, 0);

    return "Branches: " + std::to_string(numberOfBranches) + ", Depth: " + std::to_string(depth) + ", Volume: " + std::to_string(volume);
}
