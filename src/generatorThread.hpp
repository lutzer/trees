//
//  generatorThread.hpp
//  trees
//
//  Created by Lutz on 05/02/17.
//
//

#ifndef generatorThread_hpp
#define generatorThread_hpp

#include <stdio.h>
#include <vector>
#include "ofMain.h"

#include "tree.hpp"
#include "environment.hpp"

class GeneratorThread : public ofThread {


    void threadedFunction();

    env::Environment environment;
    trees::TreeParameters params;
    int iterations;

public:
    GeneratorThread(env::Environment environment, trees::TreeParameters params, int iterations);

    vector<trees::Tree> treeList;
};

#endif /* generatorThread_hpp */
