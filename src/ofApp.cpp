#include "ofApp.h"
#include "generator.hpp"

using namespace trees;

static const int GROUND_SIZE = 100;
static const int UPDATE_INTERVAL = 200;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();

    // this sets the camera's distance from the object
    cam.setDistance(100);

    // setup gui
    gui.setup("GUI");
    gui.add(iterationSlider.setup("Iterations", 50, 0, 100));
    iterationSlider.addListener(this, &ofApp::iterationSliderChanged);

    // create ground mesh
    groundMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,-GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(GROUND_SIZE/2,GROUND_SIZE/2,0));
    groundMesh.addVertex(ofPoint(-GROUND_SIZE/2,GROUND_SIZE/2,0));

    // generate Tree
    tree = generateSapling();

    // create tree Mesh

    //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    //glPointSize(3);// make the points bigger

    
    lastUpdateTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){

    if ( ofGetElapsedTimeMillis() - lastUpdateTime > UPDATE_INTERVAL ) {
        // grow tree once
        tree = iterateTree(tree,{0,0});
        treeMesh = TreeModel(tree).getMesh();
        lastUpdateTime = ofGetElapsedTimeMillis();
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    cam.begin();
    groundMesh.draw();
    treeMesh.draw();
    cam.end();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    std::cout << value << "/n";
}
