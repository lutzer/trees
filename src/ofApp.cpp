#include "ofApp.h"
#include "generator.hpp"

using namespace trees;

static const int GROUND_SIZE = 100;
static const int UPDATE_INTERVAL = 200;
static const int MAX_ITERATIONS = 100;
static const int PADDING = 30;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    // setup gui
    iterationSlider = new ofxDatGuiSlider(iteration.set("Iterations", 50, 0, MAX_ITERATIONS));
    iterationSlider->setWidth(ofGetWidth(), .2); // make label area 20% of width //
    iterationSlider->setPosition(0, ofGetHeight() - iterationSlider->getHeight());
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

    // create tree Meshes
    for (int i=0; i<=MAX_ITERATIONS; i++) {
        // grow tree once
        tree = iterateTree(tree,{0,0});
        treeMeshList.push_back(TreeModel(tree).getMesh());
    }

    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    glPointSize(3);// make the points bigger
    
    lastUpdateTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){
    iterationSlider->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    ofEnableDepthTest();
    cam.begin();
    groundMesh.draw();
    treeMeshList[iteration].draw();
    cam.end();

    // draw gui
    ofDisableDepthTest();
    ofDrawBitmapString("<a>: -iteration, <s>: +iteration", PADDING, PADDING);
    iterationSlider->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        iteration = max(iteration.getMin(),iteration -1);
    } else if (key == 's') {
        iteration = min(iteration.getMax(),iteration + 1);
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
