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
#include <functional>
#include "ofMain.h"

#include "tree.hpp"
#include "environment.hpp"

typedef std::function<void(trees::Tree)> CallbackFunction;


class GeneratorThread : public ofThread {

    void threadedFunction();

    env::Environment environment;
    trees::TreeParameters params;
    int iterations;

public:
    GeneratorThread(env::Environment environment, trees::TreeParameters params, int iterations);

    //void setIterationEventHandler(CallbackFunction iterationEventHandler);

    // holds the pointer to the function which is called after each iterations
    CallbackFunction iterationEventHandler = 0;
};

#endif /* generatorThread_hpp */
