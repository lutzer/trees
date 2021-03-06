#include "ofApp.h"

#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <functional>
#include <math.h>

#include "generator.hpp"
#include "binModel.hpp"
#include "treeModel.hpp"
#include "phototropism.hpp"
#include "environment.hpp"
#include "utils.hpp"

using namespace trees;
using namespace std;

static const int GROUND_SIZE = 100;
static const int MAX_ITERATIONS = 100;
static const int PADDING = 30;
static const int SUN_RADIUS = 5;

// Set up environment.
static const pts::BoundingBox PT_BOUNDINGBOX = { { -100, 0 }, { 200, 150 } };
static const pts::Point SUN_POSITION = { -100, 150 };
static const env::Environment environment(SUN_POSITION, PT_BOUNDINGBOX);

// Gui Labels
static const string LABEL_BRANCH_POSSIBILITY = "branch prob";
static const string LABEL_BRANCH_ANGLE = "branch angle";
static const string LABEL_BRANCH_VAR = "branch var";
static const string LABEL_BRANCH_POSITION = "branch pos";
static const string LABEL_BRANCHOUT_LENGTH = "branch length";
static const string LABEL_GROWTH_RATE = "growth rate";
static const string LABEL_TOGGLE_GRAVITY_BUTTON = "ENABLE GRAVITY";
static const string LABEL_BRANCH_FLEXIBILITY = "flexibility";
static const string LABEL_REDRAW_BUTTON = "REDRAW";

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);

    // Set default tree parameters.
    treeParams = trees::TreeParameters();

    // Setup GUI.
    iterationSlider = new ofxDatGuiSlider(iteration.set("Iterations", MAX_ITERATIONS, 0, MAX_ITERATIONS));
    iterationSlider->onSliderEvent(this, &ofApp::iterationSliderChanged);
    iteration.addListener(this, &ofApp::onIterationChanged);
    parameterFolder = new ofxDatGuiFolder("Parameters", ofColor::fromHex(0xFFD00B));
    parameterFolder->addSlider(LABEL_BRANCH_POSSIBILITY, 0.0, 1.0, treeParams.branchPossibility);
    parameterFolder->addSlider(LABEL_BRANCH_POSITION, 1.0, 16.0, treeParams.branchoutPosition);
    parameterFolder->addSlider(LABEL_BRANCHOUT_LENGTH, 0.0, 100.0, treeParams.branchoutLength);
    parameterFolder->addSlider(LABEL_BRANCH_ANGLE, 0, 1.0, treeParams.branchoutAngleMean);
    parameterFolder->addSlider(LABEL_BRANCH_VAR, 0.0, M_PI_2, treeParams.branchoutAngleStdDeviation);
    parameterFolder->addSlider(LABEL_GROWTH_RATE, 0.0, 1.0, treeParams.growthRate);
    parameterFolder->addToggle(LABEL_TOGGLE_GRAVITY_BUTTON)->setChecked(treeParams.enableGravity);
    parameterFolder->addSlider(LABEL_BRANCH_FLEXIBILITY, 0.1, 20.0, treeParams.springConstant);
    parameterFolder->addButton(LABEL_REDRAW_BUTTON);
    parameterFolder->onButtonEvent(this, &ofApp::onParamsButtonEvent);
    parameterFolder->onSliderEvent(this, &ofApp::onParamsSliderEvent);
    parameterFolder->expand();



    // Set the camera's initial position.
    cam.setDistance(200);
    cam.move(0, 50, 0);

    // Create ground mesh.
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE / 2, 0, -GROUND_SIZE / 2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE / 2, 0, -GROUND_SIZE / 2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE / 2, 0, GROUND_SIZE / 2));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE / 2, 0, GROUND_SIZE / 2));

    // calculate the tree
    this->calculateTree();

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
    parameterFolder->update();

    if (updateScene && !treeList.empty()) {

        const auto index = std::min((int)iteration,(int)treeList.size()-1);

        // Update tree mesh.
        treeMeshes = TreeModel(treeList[index]).getMeshes();

        //update info
        treeInfoString = treeList[index].toString();

        // Update bins.
        env::Bins bins = gen::calculateLightBins(treeList[index], environment);
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
    for (auto mesh : treeMeshes)
        mesh.draw();

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
        parameterFolder->draw();
        // draw status bar
        float percentage = std::min((float) treeList.size() / MAX_ITERATIONS, 1.0F);
        ofDrawBitmapString(treeInfoString, PADDING, ofGetHeight() - iterationSlider->getHeight() - PADDING );
        ofDrawBitmapString(ofToString(percentage*100) + "%", ofGetWidth() - PADDING - 30, ofGetHeight() - iterationSlider->getHeight() - PADDING );
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

    ofPoint mousePos = ofPoint(x,y);

    // if cursor is on top of a gui element, disable moving the camera
    if (utils::containsPoint(ofRectangle( iterationSlider->getX(), iterationSlider->getY(), iterationSlider->getWidth(), iterationSlider->getHeight()), mousePos) ||
        utils::containsPoint(ofRectangle( parameterFolder->getX(), parameterFolder->getY(), parameterFolder->getWidth(), parameterFolder->getHeight()), mousePos))
    {
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
    parameterFolder->setPosition(ofGetWidth() - parameterFolder->getWidth(), 0);
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

void ofApp::onParamsButtonEvent(ofxDatGuiButtonEvent e) {
    if (e.target->getName() == LABEL_REDRAW_BUTTON) {
        this->calculateTree();
    } else if (e.target->getName() == LABEL_TOGGLE_GRAVITY_BUTTON) {
        treeParams.enableGravity = !treeParams.enableGravity;
    }

}

void ofApp::onParamsSliderEvent(ofxDatGuiSliderEvent e) {

    string sliderName = e.target->getName();

    // update tree params
    if (sliderName == LABEL_GROWTH_RATE)
        treeParams.growthRate = e.target->getValue();
    else if (sliderName == LABEL_BRANCH_VAR)
        treeParams.branchoutAngleStdDeviation = e.target->getValue();
    else if (sliderName == LABEL_BRANCH_ANGLE)
        treeParams.branchoutAngleMean = e.target->getValue();
    else if (sliderName == LABEL_BRANCH_POSSIBILITY)
        treeParams.branchPossibility = e.target->getValue();
    else if (sliderName == LABEL_BRANCHOUT_LENGTH)
        treeParams.branchoutLength = e.target->getValue();
    else if (sliderName == LABEL_BRANCH_FLEXIBILITY)
        treeParams.springConstant = e.target->getValue();
    else if (sliderName == LABEL_BRANCH_POSITION)
        treeParams.branchoutPosition = e.target->getValue();
}

void ofApp::onNewIterationCalculated(trees::Tree &tree) {
    treeList.push_back(tree);
    updateScene = true;
}

void ofApp::calculateTree() {

    // stop & delete thread
    if (generatorThread != 0) {
        generatorThread->stopThread();
        generatorThread->waitForThread();
        delete generatorThread;
    }

    treeList.clear();

    // create new thread
    generatorThread = new GeneratorThread(environment, treeParams, MAX_ITERATIONS);
    generatorThread->newTreeGeneratedHandler.add(this, &ofApp::onNewIterationCalculated, 1);
    generatorThread->startThread();

}
