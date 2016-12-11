#include "ofApp.h"
#include "generator.hpp"
#include <iostream>
#include "binMesh.hpp"


using namespace trees;
using namespace std;

static const int GROUND_SIZE = 100;
static const int UPDATE_INTERVAL = 200;
static const int MAX_ITERATIONS = 80;
static const int PADDING = 30;

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

    // generate Tree Sappling
    tree = generateSapling();
    sun = {50,0};

    cout << "Generating Tree" << endl;
    // create tree Meshes
    for (int i = 0; i <= MAX_ITERATIONS; i++) {
        // grow tree once
        tree = gen::iterateTree(tree, { 0,0 });
        treeMeshList.push_back(TreeModel(tree).getMesh());
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

    bins = gen::lightBinsFromTree(treeList[iteration], sun, {{-100,0},{100,0}});
    BinMesh binVisualisation = BinMesh(bins.densities.data(), photo::binsPerAxis, photo::binsPerAxis);
    binMesh = binVisualisation.getMesh(ofPoint(-100,0,0), ofPoint(5,5,5));

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    ofEnableDepthTest();
    cam.begin();
    groundMesh.draw();
    treeMeshList[iteration].draw();
    binMesh.draw();
    cam.end();

    // draw gui
    ofDisableDepthTest();
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
