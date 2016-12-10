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
    gui.setup("GUI");
    gui.setPosition(PADDING, 50);
    gui.add(iterationSlider.setup("Iterations", 50, 0, MAX_ITERATIONS-1));
    iterationSlider.addListener(this, &ofApp::iterationSliderChanged);

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();

    // this sets the camera's distance from the object
    cam.setDistance(100);

    // create ground mesh
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,GROUND_SIZE/2,0));

    // generate Tree Sappling
    tree = generateSapling();

    // create tree Meshes
    for (int i=0; i<MAX_ITERATIONS; i++) {
        // grow tree once
        tree = iterateTree(tree,{0,0});
        treeMeshList.push_back(TreeModel(tree).getMesh());
    }

    //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    //glPointSize(3);// make the points bigger
    
    lastUpdateTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    ofDrawBitmapString("<a>: -iteration, <s>: +iteration", PADDING, PADDING);

    // even points can overlap with each other, let's avoid that
    cam.begin();
    groundMesh.draw();
    treeMeshList[iteration].draw();
    cam.end();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        iteration = max(0,iteration - 1);
    } else if (key == 's') {
        iteration = min(MAX_ITERATIONS-1,iteration + 1);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

void ofApp::iterationSliderChanged(int& value) {
    iteration = value;
}
