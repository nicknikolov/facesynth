/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */

#include "ofApp.h"
#include <math.h>

using namespace ofxCv;

// Maxi variables
maxiOsc o1, o2, o3, o4, o5, vibrato;
maxiSVF f1, f2, f3, f4, f5, f6, f7, f8, f9, f10;
maxiFilter filter;
maxiDelayline delay;

float mouthHeight, mouthWidth, browHeight;

// Params
map<string, vector<float>> scales;
ofParameter<int> transposition;
ofParameter<float> delayFeedback;
ofParameter<int> delaySize;
ofParameter<float> delayVolume;
ofParameter<float> volume;
string currentScale;
string fromVowel;
string toVowel;

vector<tuple<string,float>> notes = {
    make_tuple("C", 130.81),
    make_tuple("C Sharp", 138.59),
    make_tuple("D", 146.83),
    make_tuple("D Sharp", 155.56),
    make_tuple("E", 164.81),
    make_tuple("F", 174.61),
    make_tuple("F Sharp", 185.00),
    make_tuple("G", 196.00),
    make_tuple("G Sharp", 207.65),
    make_tuple("A", 220.00),
    make_tuple("A Sharp", 233.08),
    make_tuple("B", 246.94)
};

struct vowel {
    float freq1;
    float freq2;
    float freq3;
    float freq4;
    float freq5;
    int gain2;
    int gain3;
    int gain4;
    int gain5;
};

map<string, vowel> vowels;

//--------------------------------------------------------------
void ofApp::setup(){
    

    // Make vowels with the approriate formant frequencies and gains.
    // Numbers references from:
    // https://www.classes.cs.uchicago.edu/archive/1999/spring/CS295/Computing_Resources/Csound/CsManual3.48b1.HTML/Appendices/table3.html

    vowel vowA;
    vowel vowE;
    vowel vowI;
    vowel vowO;
    vowel vowU;
    
    vowA.freq1 = 650;
    vowA.freq2 = 1080;
    vowA.freq3 = 2650;
    vowA.freq4 = 2900;
    vowA.freq5 = 3250;
    vowA.gain2 = -6;
    vowA.gain3 = -7;
    vowA.gain4 = -8;
    vowA.gain5 = -22;
    
    vowE.freq1 = 400;
    vowE.freq2 = 1700;
    vowE.freq3 = 2600;
    vowE.freq4 = 3200;
    vowE.freq5 = 3580;
    vowE.gain2 = -14;
    vowE.gain3 = -12;
    vowE.gain4 = -14;
    vowE.gain5 = -20;

    vowI.freq1 = 290;
    vowI.freq2 = 1870;
    vowI.freq3 = 2800;
    vowI.freq4 = 3250;
    vowI.freq5 = 3540;
    vowI.gain2 = -15;
    vowI.gain3 = -18;
    vowI.gain4 = -20;
    vowI.gain5 = -30;

    vowO.freq1 = 400;
    vowO.freq2 = 800;
    vowO.freq3 = 2600;
    vowO.freq4 = 2800;
    vowO.freq5 = 3000;
    vowO.gain2 = -10;
    vowO.gain3 = -12;
    vowO.gain4 = -12;
    vowO.gain5 = -26;
    
    
    vowU.freq1 = 350;
    vowU.freq2 = 600;
    vowU.freq3 = 2700;
    vowU.freq4 = 2900;
    vowU.freq5 = 3300;
        
    vowU.gain2 = -20;
    vowU.gain3 = -17;
    vowU.gain4 = -14;
    vowU.gain5 = -26;
    
    vowels["A"] = vowA;
    vowels["E"] = vowE;
    vowels["I"] = vowI;
    vowels["O"] = vowO;
    vowels["U"] = vowU;

    fromVowel = "A";
    toVowel = "E";
    
    // Generate scales procedurally!
    // Calcuate each scale based on pregiven major and minor indices
    // Store them in scales map for easy referencing later
    for (auto i=0; i<=11; i++) {
        int multiplier;
        vector<int> majorScaleNotes = {0, 2, 4, 5, 7, 9, 11};
        vector<int> minorScaleNotes = {0, 2, 3, 5, 7, 8, 10};
        string scaleName;
        vector<float> major, minor;

        // Make a major scale ----------------------------------
        for (auto & majorScaleNote : majorScaleNotes) {
            int index = majorScaleNote + i;
            index = index % 12;
            multiplier = (int)((majorScaleNote + i) / 12) + 1;
            major.push_back(get<1>(notes[index]) * multiplier);
        }
        
        scaleName = get<0>(notes[0 + i]) + " Major";
        scales[scaleName] = major;
        // ----------------------------------------------------
        
        
        
        // Make a minor scale ---------------------------------
        for (auto & minorScaleNote : minorScaleNotes) {
            int index = minorScaleNote + i;
            index = index % 12;
            multiplier = (int)((minorScaleNote + i) / 12) + 1;
            minor.push_back(get<1>(notes[index]) * multiplier);
        }
        
        scaleName = get<0>(notes[0 + i]) + " Minor";
        scales[scaleName] = minor;
        // ----------------------------------------------------
        
    }
    
    // Print scales
//    for(auto & scale : scales) {
//        cout << "Scale: " << scale.first << endl;
//        for (auto & note : scale.second) {
//            cout << note << endl;
//        }
//    }
    
    // GUI
    gui.setup("panel");
    gui.add(transposition.set("transposiiton", 1, -2, 5));
    gui.add(delayFeedback.set("delay feedback", 0.5, 0, 0.9));
    gui.add(delaySize.set("delay size", 50, 0, 100));
    gui.add(delayVolume.set("delay volume", 0.4, 0, 1));
    gui.add(volume.set("volume", 0.6, 0, 1));
    
    currentScale = "C Major";
    
    sampleRate 	= 44100;
	bufferSize	= 512;

    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    
    ofEnableAlphaBlending();
    ofBackground(20, 20, 20);
    ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    
    cam.initGrabber(640, 480);
    tracker.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cam.update();
    if(cam.isFrameNew()) {
        tracker.update(toCv(cam));
        position = tracker.getPosition();
        scale = tracker.getScale();
        orientation = tracker.getOrientation();
        rotationMatrix = tracker.getRotationMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    cam.draw(384, 0);
    ofSetColor(255);
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetWindowHeight()-25);
    ofDrawBitmapString(currentScale, 30, ofGetWindowHeight()-25);
    ofDrawBitmapString("from vowel: " + fromVowel, 95, ofGetWindowHeight()-25);
    ofDrawBitmapString("to vowel: " + toVowel, 210, ofGetWindowHeight()-25);
    if(tracker.getFound()) {
        ofSetLineWidth(1);
        ofPushMatrix();
        ofTranslate(-140, 0);
        tracker.draw();
        ofPopMatrix();
        
        mouthHeight = tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT);
        mouthWidth = tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH);
        browHeight = tracker.getGesture(ofxFaceTracker::LEFT_EYEBROW_HEIGHT);
    }
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    // Map the mouth height to any of the notes in the current scale
    
    int mouthHeightNormalized =  ((int)mouthHeight - 1);
    int note = mouthHeightNormalized % 7;
    if (note < 0) note = 0;
    float localTransposition = transposition + (mouthHeightNormalized / 7);

    for (int i = 0; i < bufferSize; i++) {
        
        // Add Vibrato from the eyebrow height
        float freq = scales[currentScale][note];
        for (int i=0; i<abs(localTransposition); i++) {
            if (localTransposition >= 0) freq *= 2;
            else freq /= 2;
        }
        float vibr = (vibrato.sinewave(10) * 10) * ((browHeight - 7) / 3);
        float osc = o1.saw(freq + vibr);
        float res = 20;
        
        // Lerp between vowels depending on the mouth width
        float amt = (mouthWidth - 10) / 10;
        float f1freq, f2freq, f3freq, f4freq, f5freq;
        float gain2, gain3, gain4, gain5;

        vowel from = vowels[fromVowel];
        vowel to = vowels[toVowel];
        
        f1freq = ofLerp(from.freq1, to.freq1, amt);
        f2freq = ofLerp(from.freq2, to.freq2, amt);
        f3freq = ofLerp(from.freq3, to.freq3, amt);
        f4freq = ofLerp(from.freq4, to.freq4, amt);
        f5freq = ofLerp(from.freq5, to.freq5, amt);
        
        gain2 = ofLerp(dbToVoltage(from.gain2), dbToVoltage(to.gain2), amt);
        gain3 = ofLerp(dbToVoltage(from.gain3), dbToVoltage(to.gain3), amt);
        gain4 = ofLerp(dbToVoltage(from.gain4), dbToVoltage(to.gain4), amt);
        gain5 = ofLerp(dbToVoltage(from.gain5), dbToVoltage(to.gain5), amt);

        // Formant filter implementation
        float out1 = f1.setCutoff(f1freq).setResonance(res).play(osc, 0.0, 1.0, 0.0, 0.0);
        float out2 = f2.setCutoff(f2freq).setResonance(res).play(osc, 0.0, 1.0, 0.0, 0.0) * gain2;
        float out3 = f3.setCutoff(f3freq).setResonance(res).play(osc, 0.0, 1.0, 0.0, 0.0) * gain3;
        float out4 = f4.setCutoff(f4freq).setResonance(res).play(osc, 0.0, 1.0, 0.0, 0.0) * gain4;
        float out5 = f5.setCutoff(f5freq).setResonance(res).play(osc, 0.0, 1.0, 0.0, 0.0) * gain5;
        
        float out = (out1 + out2 + out3 + out4 + out5) / 5;
        out *= volume;
        
        // Shut up if mouth is shut
        if (mouthHeight < 1.5) out = 0;
        
        // Add a bit of delay to make more interesting melodies
        float delayOut = delay.dl(out, delaySize * 1000, delayFeedback) * delayVolume;

        output[i*nChannels    ] = out + delayOut;
        output[i*nChannels + 1] = out + delayOut;
	}
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r') {
        tracker.reset();
    }
    
    if (key == 'x') {
        transposition++;
    }
    
    
    if (key == 'z') {
        transposition--;
    }
    
    if (key == 'a') {
        currentScale = "C Minor";
    }
    
    if (key == 'A') {
        currentScale = "C Major";
    }
    
    if (key == 'w') {
        currentScale = "C Sharp Minor";
    }
    
    if (key == 'W') {
        currentScale = "C Sharp Major";
    }
    
    if (key == 's') {
        currentScale = "D Minor";
    }
    
    if (key == 'S') {
        currentScale = "D Major";
    }
    
    if (key == 'e') {
        currentScale = "D Sharp Minor";
    }
    
    if (key == 'E') {
        currentScale = "D Sharp Major";
    }
    
    if (key == 'd') {
        currentScale = "E Minor";
    }
    
    if (key == 'D') {
        currentScale = "E Major";
    }
    
    if (key == 'f') {
        currentScale = "F Minor";
    }
    
    if (key == 'F') {
        currentScale = "F Major";
    }
    
    if (key == 't') {
        currentScale = "F Sharp Minor";
    }
    
    if (key == 'T') {
        currentScale = "F Sharp Major";
    }
    
    if (key == 'g') {
        currentScale = "G Minor";
    }
    
    if (key == 'G') {
        currentScale = "G Major";
    }
    
    if (key == 'y') {
        currentScale = "G Sharp Minor";
    }
    
    if (key == 'Y') {
        currentScale = "G Sharp Major";
    }
    
    if (key == 'h') {
        currentScale = "A Minor";
    }
    
    if (key == 'H') {
        currentScale = "A Major";
    }
    
    if (key == 'u') {
        currentScale = "A Sharp Minor";
    }
    
    if (key == 'U') {
        currentScale = "A Sharp Major";
    }
    
    if (key == 'j') {
        currentScale = "B Minor";
    }
    
    if (key == 'J') {
        currentScale = "B Major";
    }
    
    if (key == 'c') {
        fromVowel = "A";
    }
    
    if (key == 'C') {
        toVowel = "A";
    }
    
    if (key == 'v') {
        fromVowel = "E";
    }
    
    if (key == 'V') {
        toVowel = "E";
    }
    
    if (key == 'b') {
        fromVowel = "I";
    }
    
    if (key == 'B') {
        toVowel = "I";
    }
    
    if (key == 'n') {
        fromVowel = "O";
    }
    
    if (key == 'N') {
        toVowel = "O";
    }
    
    if (key == 'm') {
        fromVowel = "U";
    }
    
    if (key == 'M') {
        toVowel = "U";
    }
}

float ofApp::dbToVoltage(float db) {
    return pow(10, db / 20);
}
