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

TreeParameters::TreeParameters(double branchoutAngleMean, double branchoutAngleVariance, double branchPossibility, double growthRate) {
    this->branchoutAngleMean = branchoutAngleMean;
    this->branchoutAngleVariance = branchoutAngleVariance;
    this->growthRate = growthRate;
    this->branchPossibility = branchPossibility;
}

TreeParameters::TreeParameters() {
    TreeParameters(1, 1, 1, 1);
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
