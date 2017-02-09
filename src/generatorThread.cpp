//
//  generatorThread.cpp
//  trees
//
//  Created by Lutz on 05/02/17.
//
//

#include "generatorThread.hpp"

#include "generator.hpp"

GeneratorThread::GeneratorThread(env::Environment environment, trees::TreeParameters params, int iterations, bool enableGravity) {
    this->environment = environment;
    this->params = params;
    this->iterations = iterations;
    this->enableGravity = enableGravity;
}

void GeneratorThread::threadedFunction() {

    // Generate sapling.
    pts::Point treeOrigin = { environment.boundingBox.origin.x + environment.boundingBox.size.width / 2 };
    trees::Tree tree = trees::Tree(treeOrigin, M_PI_2, params);

    cout << "Generating tree" << endl;
    // Create tree iterations.
    for (int i = 0; i <= iterations; i++) {
        // Calculate light.
        env::Bins bins = gen::calculateLightBins(tree, environment);

        // Grow tree once.
        tree = gen::iterateTree(tree, bins, environment);

        // calculate gravitational forces
        if (this->enableGravity)
            gen::applyGravitationalForce(tree, environment);

        if (!isThreadRunning())
            break;

        // send event
        newTreeGeneratedHandler.notify(this, tree);
    }
    cout << "Tree generation finished" << endl;
    cout << tree.toString() << endl;
}
