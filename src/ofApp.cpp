#include "ofApp.h"

#include <iostream>
#include <algorithm>

#include "generator.hpp"
#include "binModel.hpp"
#include "treeModel.hpp"
#include "phototropism.hpp"
#include "environment.hpp"

using namespace trees;
using namespace std;

static const int GROUND_SIZE = 100;
static const int MAX_ITERATIONS = 80;
static const int PADDING = 30;
static const int SUN_RADIUS = 3;

// Set up environment.
static const pts::BoundingBox PT_BOUNDINGBOX = { { -50, 0 }, { 100, 100 } };
static const pts::Point SUN_POSITION = { -50, 0 };
static const env::Environment environment(SUN_POSITION, PT_BOUNDINGBOX);

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);

    // Setup GUI.
    iterationSlider = new ofxDatGuiSlider(iteration.set("Iterations", 50, 0, MAX_ITERATIONS));
    iterationSlider->onSliderEvent(this, &ofApp::iterationSliderChanged);
    iteration.addListener(this, &ofApp::onIterationChanged);

    // Set the camera's initial position.
    cam.setDistance(200);
    cam.move(0, 50, 0);

    // Create ground mesh.
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2, 0, -GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2, 0, -GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2, 0, GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2, 0, GROUND_SIZE/2));

    // Set tree parameters.
    const double branchoutAngleMean = 0.0;
    const double branchoutAngleVariance = 0.5;
    const double branchPossibility = 0.08;
    const double growthRate = 0.3;
    trees::TreeParameters params(branchoutAngleMean, branchoutAngleVariance, branchPossibility, growthRate);

    // Generate sapling.
    pts::Point treeOrigin = { PT_BOUNDINGBOX.origin.x + PT_BOUNDINGBOX.size.width / 2 };
    trees::Tree tree = trees::Tree(treeOrigin, params);

    cout << "Generating Tree" << endl;
    // Create tree meshes.
    for (int i = 0; i <= MAX_ITERATIONS; i++) {
        // Calculate light.
        env::Bins bins = gen::calculateLightBins(tree, environment);

        // Grow tree once.
        tree = gen::iterateTree(tree, bins, environment);
        treeList.push_back(tree);
        cout << "Generated iteration " << i << endl;
    }

    glEnable(GL_POINT_SMOOTH); // Use circular points instead of square points.
    glPointSize(3); // Make the points bigger.

    // Set up parameters.
    lastUpdateTime = ofGetElapsedTimeMillis();

    showBins = DENSITIES;
    showGui = true;

    this->windowResized(ofGetWidth(), ofGetHeight());

    updateScene = true;
}

//--------------------------------------------------------------
void ofApp::update() {
    iterationSlider->update();

    if (updateScene) {
        // Update tree mesh.
        treeMesh = TreeModel(treeList[iteration]).getMesh();

        // Update bins.
        env::Bins bins = gen::calculateLightBins(treeList[iteration], environment);
        BinModel binModel = (showBins == LIGHT) ?
            BinModel(bins.light.data(), bins.size.columns, bins.size.rows) :
            BinModel(bins.densities.data(), bins.size.columns, bins.size.rows);

        ofPoint origin = ofPoint(PT_BOUNDINGBOX.origin.x, PT_BOUNDINGBOX.origin.y, 0);
        ofVec3f size = ofVec3f(PT_BOUNDINGBOX.size.width, PT_BOUNDINGBOX.size.height, 0);
        binMesh = binModel.getMesh(origin, size);

        updateScene = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // Draw 3d scene.
    ofEnableDepthTest();
    cam.begin();
    groundMesh.draw();
    treeMesh.draw();

    // Draw sun.
    ofDrawSphere(environment.sun.x, environment.sun.y, 0, SUN_RADIUS);

    if (showBins != HIDDEN) {
        binMesh.draw();
    }
    cam.end();
    ofDisableDepthTest();

    // Draw GUI.
    if (showGui) {
        ofDrawBitmapString("<a>: -iteration, <s>: +iteration\n"
                           "<f>: toggle Fullscreen\n"
                           "<b>: switch through Bins\n"
                           "<g>: toggle GUI",
                           PADDING, PADDING);
        iterationSlider->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case 'a':
            iteration = max(iteration.getMin(), iteration -1);
            break;
        case 's':
            iteration = min(iteration.getMax(), iteration + 1);
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'b':
            showBins = BinVisibilty((showBins + 1) % 3);
            updateScene = true;
            break;
        case 'g':
            showGui = !showGui;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {
    if (y > iterationSlider->getY()) {
        cam.disableMouseInput();
    } else {
        cam.enableMouseInput();
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    iterationSlider->setWidth(ofGetWidth(), .1);
    iterationSlider->setPosition(0, ofGetHeight() - iterationSlider->getHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

void ofApp::onIterationChanged(int &value) {
    updateScene = true;
}

void ofApp::iterationSliderChanged(ofxDatGuiSliderEvent e) {
    iteration = e.value;
}
