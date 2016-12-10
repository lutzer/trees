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

namespace pts {
    struct Point {
        double x, y;
    };
}

namespace trees {
    struct Branch {
        double angle, length, thickness;
        vector<Branch> children;
    };

    struct Tree {
        Branch base;
        pts::Point origin;
    };
}


trees::Tree generateSapling();

#endif /* tree_hpp */
