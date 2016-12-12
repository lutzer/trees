#include "ofApp.h"
#include "generator.hpp"
#include <iostream>
#include "binModel.hpp"
#include "treeModel.hpp"
#include "phototropism.hpp"

using namespace trees;
using namespace std;

static const int GROUND_SIZE = 100;
static const int UPDATE_INTERVAL = 200;
static const int MAX_ITERATIONS = 80;
static const int PADDING = 30;
static const pts::BoundingBox PT_BOUNDINGBOX = {{-50,0},{100,100}};

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    // setup gui
    iterationSlider = new ofxDatGuiSlider(iteration.set("Iterations", 50, 0, MAX_ITERATIONS));
    iterationSlider->onSliderEvent(this, &ofApp::iterationSliderChanged);
    iteration.addListener(this, &ofApp::onIterationChanged);

    // set the camera's initial position
    cam.setDistance(200);
    cam.move(0, 50, 0);

    // create ground mesh
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2, 0, -GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2, 0, -GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2, 0, GROUND_SIZE/2));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2, 0, GROUND_SIZE/2));

    // Set Sun Position
    sun = {0,200};

    // generate Tree Sappling
    trees::Tree tree = generateSapling({0,0});

    cout << "Generating Tree" << endl;
    // create tree Meshes
    for (int i = 0; i <= MAX_ITERATIONS; i++) {
        // grow tree once
        tree = gen::iterateTree(tree, { 0,0 });
        treeList.push_back(tree);
        cout << "Generated iteration " << i << endl;
    }

    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    glPointSize(3);// make the points bigger

    // setup parameters
    lastUpdateTime = ofGetElapsedTimeMillis();
    showBins = true;

    this->windowResized(ofGetWidth(), ofGetHeight());

    updateScene = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    iterationSlider->update();

    if (updateScene) {

        // udate tree mesh
        treeMesh = TreeModel(treeList[iteration]).getMesh();

        // update bins
        photo::LightBins bins = gen::lightBinsFromTree(treeList[iteration], sun, PT_BOUNDINGBOX);
        BinModel binModel = BinModel(bins.densities.data(), photo::binsPerAxis, photo::binsPerAxis);

        ofPoint origin = ofPoint(PT_BOUNDINGBOX.origin.x,PT_BOUNDINGBOX.origin.y,0);
        ofVec3f size = ofVec3f(PT_BOUNDINGBOX.size.width,PT_BOUNDINGBOX.size.height,0);
        binMesh = binModel.getMesh(origin, size);

        updateScene = false;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    ofEnableDepthTest();
    cam.begin();
    groundMesh.draw();
    treeMesh.draw();
    if (showBins)
        binMesh.draw();
    cam.end();
    ofDisableDepthTest();

    // draw gui
    ofDrawBitmapString("<a>: -iteration, <s>: +iteration \n"
                       "<f>: toggle Fullscreen \n"
                       "<b>: toggle Bins",
                       PADDING, PADDING);
    iterationSlider->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
        case 'a':
            iteration = max(iteration.getMin(),iteration -1);
            break;
        case 's':
            iteration = min(iteration.getMax(),iteration + 1);
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'b':
            showBins = !showBins;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (y>iterationSlider->getY())
        cam.disableMouseInput();
    else
        cam.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    iterationSlider->setWidth(ofGetWidth(), .1);
    iterationSlider->setPosition(0, ofGetHeight() - iterationSlider->getHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::onIterationChanged(int &value) {
    updateScene = true;
}

void ofApp::iterationSliderChanged(ofxDatGuiSliderEvent e) {
    iteration = e.value;
}
