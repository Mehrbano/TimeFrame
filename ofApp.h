#pragma once
/*
 --------------- ATTENTION --------------
 RUNTIME ERROR FIX
 BUG FIXED BY ADDING THE FOLLOWING LINE
 if( !bAllocated || ( mom->nChannels == cvImage->nChannels && mom->depth == cvImage->depth ) ){
 IN PLACE OF
 if( mom->nChannels == cvImage->nChannels && mom->depth == cvImage->depth ){
 --------------- ABOUT PROGRAM --------------
 THIS PROGRAM IS INTERACTIVE WITH THE VIEWER'S HAND GESTURES ON X AND Y AXES USING OPTICALFLOWFARNEBACK
 MY OWN VIDEOS AND IMAGES FROM EXPERIMENTS & ARTWORKS ARE USED
 ---------------  CREDITS --------------
 *  ofxOpticalFlowFarneback created by Timothy Scaffidi on 13/11/2012 based on ofxOpticalFlowLK by lukasz karluk.
 *  Mastering openFrameworks - Creative Coding Demystified (Denis Perevalov).pdf
 *  GITHUB
 */
//--------------------------
#include "ofMain.h"
#include "ofxOpenCv.h"
using namespace cv;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void frame (float phase);
    void motif (float len);
    
    ofVideoGrabber video;
    ofVideoPlayer vidply, vidply2 ;
    
    ofxCvColorImage currentColor;		//ORIGINAL IMAGE
    ofxCvGrayscaleImage gray1, gray2;	//DECIMATE IMAGE
    ofxCvFloatImage flowX, flowY;		//RESULT OF FLOW ON x & y AXES
    float sumX, sumY, avgX, avgY;
    float phase, time,t;
    int numOfEntries;
    int x, y;
    int theAlpha;                       //FROM GITHUB
    
    
    bool debug=false;
    bool calFlow;
    
    ofImage image2, image;
    
};
