#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxCv.h"
#include "ofxGui.h"

using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    float dbToVoltage(float db);
	
    
    int             bufferSize;
    int             sampleRate;
    ofVideoGrabber  cam;
    ofxFaceTracker  tracker;
    ofVec2f         position;
    float           scale;
    ofVec3f         orientation;
    ofMatrix4x4     rotationMatrix;
    Mat             translation, rotation;
    ofMatrix4x4     pose;
    ofEasyCam       easyCam;
    
    ofxPanel        gui;

};


