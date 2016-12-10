#include "ofApp.h"
#include "tree.hpp"

using namespace trees;

#define GROUND_SIZE 100

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

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

    // generate Tree
    TreeMesh tree = TreeMesh(generateSapling());

    // create tree Mesh
    treeMesh = tree.getMesh();

    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    glPointSize(3);// make the points bigger
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    cam.begin();
    groundMesh.draw();
    treeMesh.draw();
    cam.end();

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
