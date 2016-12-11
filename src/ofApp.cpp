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

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    //ofEnableDepthTest();

    // this sets the camera's distance from the object
    cam.setDistance(200);

    // create ground mesh
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,GROUND_SIZE/2,0));

    // Setup Sun Position
    sun = {0,75};

    // generate Tree Sappling
    tree = generateSapling();

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
    
    lastUpdateTime = ofGetElapsedTimeMillis();

    this->windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    iterationSlider->update();

    photo::LightBins bins = gen::lightBinsFromTree(treeList[iteration], sun, PT_BOUNDINGBOX);
    BinModel binModel = BinModel(bins.densities.data(), photo::binsPerAxis, photo::binsPerAxis);

    ofPoint origin = ofPoint(PT_BOUNDINGBOX.origin.x,0,PT_BOUNDINGBOX.origin.y);
    ofVec3f size = ofVec3f(PT_BOUNDINGBOX.size.width,0,PT_BOUNDINGBOX.size.height);
    binMesh = binModel.getMesh(origin, size);

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    ofEnableDepthTest();
    cam.begin();
    groundMesh.draw();
    TreeModel(treeList[iteration]).getMesh().draw();
    binMesh.draw();
    cam.end();
    ofDisableDepthTest();

    // draw gui
    ofDrawBitmapString("<a>: -iteration, <s>: +iteration \n<f>: toggle Fullscreen", PADDING, PADDING);
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

void ofApp::iterationSliderChanged(ofxDatGuiSliderEvent e) {
    iteration = e.value;
}
