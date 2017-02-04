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

TreeParameters::TreeParameters(double branchoutAngleMean, double branchoutAngleVariance, double branchPossibility, double growthRate) {
    this->branchoutAngleMean = branchoutAngleMean;
    this->branchoutAngleVariance = branchoutAngleVariance;
    this->growthRate = growthRate;
    this->branchPossibility = branchPossibility;
}

Tree::Tree(pts::Point origin, TreeParameters *params) {
    this->origin = origin;
    this->params = params;
    this->base = new Branch(0, M_PI, 1, 1);
}
