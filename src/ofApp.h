#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxDatGui.h"
#include "tree.hpp"

enum BinVisibilty {
    HIDDEN = 0,
    DENSITIES = 1,
    LIGHT = 2
};

class ofApp : public ofBaseApp{
private:
    // simulation objects
    pts::Point sun;
    vector<trees::Tree> treeList;

    // scene objects
    ofEasyCam cam;
    ofMesh groundMesh;
    ofMesh binMesh;
    ofMesh treeMesh;
    bool updateScene;

    // Adjustable Parameters
    ofParameter<int> iteration;
    BinVisibilty showBins;

    // Gui
    ofxDatGuiSlider* iterationSlider;
    bool showGui;

    long lastUpdateTime;

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    /* Event handlers */
    void iterationSliderChanged(ofxDatGuiSliderEvent e);
    void onIterationChanged(int &value);
};
