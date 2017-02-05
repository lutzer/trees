//
//  generatorThread.cpp
//  trees
//
//  Created by Lutz on 05/02/17.
//
//

#include "generatorThread.hpp"

#include "generator.hpp"

GeneratorThread::GeneratorThread(env::Environment environment, trees::TreeParameters params, int iterations) {
    this->environment = environment;
    this->params = params;
    this->iterations = iterations;
}

void GeneratorThread::threadedFunction() {

    // Generate sapling.
    pts::Point treeOrigin = { environment.boundingBox.origin.x + environment.boundingBox.size.width / 2 };
    trees::Tree tree = trees::Tree(treeOrigin, params);

    cout << "Generating Tree" << endl;
    // Create tree iterations.
    for (int i = 0; i <= iterations; i++) {
        // Calculate light.
        env::Bins bins = gen::calculateLightBins(tree, environment);

        // Grow tree once.
        tree = gen::iterateTree(tree, bins, environment);
        treeList.push_back(tree);
        cout << "Generated iteration " << i << endl;
    }
    cout << "Tree generation finished" << endl;
}
