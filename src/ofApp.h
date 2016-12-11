#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "treeModel.hpp"
#include "tree.hpp"

class ofApp : public ofBaseApp{

private:
    ofEasyCam cam;
    vector<ofMesh> treeMeshList;
    trees::Tree tree;
    ofMesh groundMesh;

    long lastUpdateTime;

    // GUI
    ofParameter<int> iteration;
    ofxDatGuiSlider* iterationSlider;

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void iterationSliderChanged(ofxDatGuiSliderEvent e);
    
};
