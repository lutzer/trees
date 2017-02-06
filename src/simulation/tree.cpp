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

using namespace trees;

Branch::Branch(double position, double angle, double length, double thickness) {
    this->position = position;
    this->angle = angle;
    this->length = length;
    this->thickness = thickness;
}

Branch::Branch() {
    Branch(0, M_PI_2, 0, 1);
}

TreeParameters::TreeParameters(double branchoutAngleMean, double branchoutAngleStdDeviation, double branchPossibility, double branchoutLength, double growthRate) {
    this->branchoutAngleMean = branchoutAngleMean;
    this->branchoutAngleStdDeviation = branchoutAngleStdDeviation;
    this->growthRate = growthRate;
    this->branchPossibility = branchPossibility;
    this->branchoutLength = branchoutLength;
}

TreeParameters::TreeParameters() {
    TreeParameters(1, 1, 1, 1, 1);
}

Tree::Tree(pts::Point origin, TreeParameters params) {
    this->origin = origin;
    this->params = params;
    this->base = Branch(0, M_PI_2, 1, 1);
}

Tree::Tree() {
    this->origin = { 0, 0 };
    this->base = Branch(0, M_PI_2, 1, 1);
}

void iterateBranches(const Branch &branch, int &numberOfBranches, float &biomass, int &depth, int currentDepth) {

    depth = std::max(depth,currentDepth);
    biomass += (branch.thickness/2) * (branch.thickness/2) * M_PI * branch.length;
    numberOfBranches += branch.children.size();

    for( auto child : branch.children) {
        iterateBranches(child, numberOfBranches, biomass, depth, currentDepth + 1);
    }
}

std::string Tree::toString() {

    int numberOfBranches = 1;
    int depth = 0;
    float biomass = 0;

    // calculates depth, number of branches and biomass
    iterateBranches(this->base, numberOfBranches, biomass, depth, 0);

    return "Branches: " + std::to_string(numberOfBranches) + ", Depth: " + std::to_string(depth) + ", Biomass: " + std::to_string(biomass);
}
