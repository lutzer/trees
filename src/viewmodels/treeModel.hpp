//
//  TreeMesh.hpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#ifndef TreeMesh_hpp
#define TreeMesh_hpp

#include "tree.hpp"
#include "ofMesh.h"

class TreeModel {
public:
    TreeModel(trees::Tree &tree, bool enableGravity);
    explicit TreeModel(trees::Tree &tree);
    ofMesh getMesh();

private:
    trees::Tree tree;
    bool enableGravity;
};


#endif /* TreeMesh_hpp */
