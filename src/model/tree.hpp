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
        double angle;
        double size;
        double thickness;
        vector<Branch *> children;
    };

    struct Point {
        double x;
        double y;
    };

    struct Tree {
        Branch *base;
        Point *origin;
    };
}

#endif /* tree_hpp */
