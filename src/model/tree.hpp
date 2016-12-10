//
//  tree.hpp
//  trees
//
//  Created by Erich Grunewald on 2016-12-10.
//
//

#ifndef tree_hpp
#define tree_hpp

#include <stdio.h>
#include <vector>

using namespace std;

namespace trees {
    struct Branch {
        double angle, length, thickness;
        vector<Branch> children;
    };

    struct Point {
        double x, y;
    };

    struct Tree {
        Branch base;
        Point origin;
    };
}

trees::Tree sapling();

#endif /* tree_hpp */
