#include "ofApp.h"
#define WIDTH  1024
#define HEIGHT 768

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetBackgroundColor(255, 0,0);
    ofSetBackgroundAuto(FALSE);
//VIDEO GRABBER
    video.setDeviceID(0);
    video.setDesiredFrameRate(60);
    video.initGrabber(640, 480);
    
//VIDEO PLAYER
    vidply.load( "06_Experiment.mov" );
    vidply.play();
    vidply2.load("15_Experiment.mov");
    vidply2.play();

//IMAGES
    image2.load("mk2.png");
    image2.resize(WIDTH, HEIGHT);
    image.load("mk.jpg");
    image.resize(WIDTH, HEIGHT);

//FLOW
    calFlow = false;
    phase = 0;
    t =0;
    theAlpha = 0;
    
    }
//--------------------------------------------------------------
void ofApp::update(){
    int minutes = 2.35;
    int totalMillis = minutes * 60 * 1000;
    time = ofMap (ofGetElapsedTimeMillis(), 0, totalMillis, 0, 3, true);
    theAlpha = theAlpha - 100;
    t++;

    vidply.update();
    vidply2.update();
    
//DECODE FOR NEW FRAME
    video.update();
    if ( video.isFrameNew() ){
        if ( gray1.bAllocated ) {
            gray2 = gray1;
            calFlow = true;
        }

//CONVERTING OFXCV IMAGES
        ofPixels & pixels = video.getPixels();
        currentColor.setFromPixels( pixels );

//DECIMATE
        float decimate = 0.25;
        ofxCvColorImage imageDecimated1;
        imageDecimated1.allocate( currentColor.width * decimate, currentColor.height * decimate );
        imageDecimated1.scaleIntoMe( currentColor, CV_INTER_AREA );             //High-quality resize
        gray1 = imageDecimated1;

//CREATE IMAGE OPENCV
        if ( gray2.bAllocated ) {
            Mat img1( gray1.getCvImage() );
            Mat img2( gray2.getCvImage() );
//Image for flow
            Mat flow;

//UPDATE FLOW
            calcOpticalFlowFarneback( img1, img2, flow, 0.3, 3, 25, 5, 5, 0.3, 0 );
//SPLIT FLOW
            vector<Mat> flowPlanes;
            split( flow, flowPlanes );

//COPY FLOW TO OFXCV IMAGES
            IplImage iplX( flowPlanes[0] );
            flowX = &iplX;
            IplImage iplY( flowPlanes[1] );
            flowY = &iplY;
        }
    }
        if (time>=3.50) ofExit();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//STATICS AVG DIRECTION
    sumX = sumY = avgX = avgY = numOfEntries = 0;
    
    if (calFlow){
        ofSetColor( 255, 255, 255, theAlpha );
        video.draw(0, 0, 10, 10);
        int w = gray1.width;
        int h = gray1.height;
        
//ADDING OPTICAL FLOW
        ofPushMatrix();
        ofScale( -4, 4 );
        
        float *flowXPixels = flowX.getPixelsAsFloats();
        float *flowYPixels = flowY.getPixelsAsFloats();
        
        ofSetColor( 0, 0, 255 );
        for (int y=0; y<h; y+=5) {
            for (int x=0; x<w; x+=5) {
                float fx = flowXPixels[ x + w * y ];
                float fy = flowYPixels[ x + w * y ];
                
//VECTORS
                if ( fabs ( fx ) + fabs( fy ) > .5 ) {
                    if (debug){
                        ofDrawRectangle( x-0.5, y-0.5, 1, 1 );
                        ofDrawLine( x, y, x + fx, y + fy );
                    }
//ADDING DIRECTION
                    sumX+=fx;
                    sumY+=fy;
                    numOfEntries++;
                }
            }
        }
        ofPopMatrix();
    }
    
//CALCULATE AVG DIR VECTORS
        if (numOfEntries>0){
            avgX = sumX / numOfEntries;
            avgY = sumY / numOfEntries;
            }
        ofPoint avgDir = ofPoint(avgX, avgY);
        if (debug){
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofSetLineWidth(5);
            ofSetColor(255,0,0);
            ofDrawLine(0,0, avgDir.x, avgDir.y);
            }

//TIME CALCULATION FOR OBJECTS ON SCREEN
        if (time > 0.05 && time < 0.50) {
        phase += ofClamp (avgDir.x, avgDir.y, 40);
        int numOfframes = 100;
        for (int j = 0; j < numOfframes; j++){
            motif (500*phase);
            }
        }
    
        if (time > 0.50 && time < 1.00) {
        ofSetColor ( 100, 255, 255 );
        vidply.draw (0,0, WIDTH, HEIGHT);
        }
    
        if (time > 1.00 && time < 1.30){
        ofSetColor ( 200, 200, 255);
        vidply.draw (0,0,WIDTH, HEIGHT);
        }
    
        if (time > 1.75) {
        ofSetColor( 255, 20, 10);
        ofDrawRectangle (0, 0, WIDTH, HEIGHT);
        }
    
        if (time > 1.30 && time < 2.95) {
            phase += ofClamp(avgDir.x, -4, 4);
            int numOfframes = 10;
            for (int i=0; i<numOfframes; i++){
                frame (phase + i * 180/numOfframes);
            }
        }
    
        if (time > 2.90 && time < 3.40) {
            ofSetColor ( 255, 255, 255 );
            vidply2.draw (0,0, WIDTH, HEIGHT);
            }
    }

//--------------------------------------------------------------
void ofApp::frame(float p){
//FIRST INTERACTION WITH IMAGE
    if (time > 1.30 && time < 1.90) {
        ofPushMatrix();
        ofPushStyle();
        ofSetPolyMode(OF_POLY_WINDING_ODD);
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        int diam = ofMap (0.5, PI, 0.3, 20, 10);
        ofRotateZ(p);
        float s = abs (sin (ofDegToRad (p))) /0.3 ;
        ofScale(s,s,s);
        ofNoFill();
        ofSetColor(255,250,70);
        ofSetLineWidth(2);
        ofDrawRectangle(0, diam, 100,100);
        image2.draw(0, diam, 100,100);
        ofPopMatrix();
        ofPopStyle();
        }

//SECOND INTERACTION WITH IMAGE
    if (time > 1.80 && time < 2.30) {
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor( 70, 255, 255 );
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotateZ(-p);
        float s1 = abs(-sin(ofDegToRad(p))) + 0.3;
        ofScale(s1,s1,s1);
        image2.draw(x, y, 400, 400);
        ofPopMatrix();
        ofPopStyle();
        }

//THIRD INTERACTION WITH WEBCAM
    if (time > 2.20 && time < 2.70) {
        ofPushMatrix();
        ofPushStyle();
        ofSetColor( 255, 255, 255 );
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotateZ(p);
        float s2 = abs (-sin(ofDegToRad(p))) + 0.2;
        ofScale(s2,s2,s2);
        video.draw (0, 0, 400, ofRandom(400, 410));
        ofPopMatrix();
        ofPopStyle();
        }
    
//FOURTH INTERACTION WITH VIDEO
    if (time > 2.65 && time < 2.90) {
        
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        float size = ofMap (time, 2.50, 2.90, -5, 10, false);
        ofRotateZ(-p);
        float s = abs(sin(ofDegToRad(p))) + 0.3;
        ofScale(s,size,s);
        ofNoFill();
        ofSetColor( 70, 255, 255 );
        ofSetLineWidth(5);
        vidply.draw(x, y, 200,200);
        ofPopMatrix();
        ofPopStyle();
        }
    }
//--------------------------------------------------------------
void ofApp::motif(float len){
//ADAPTED FROM RECURSIVE TREE EXERCISE, CHANGED AND USED
    ofSetColor(255);
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    image.draw (0, 0, 20, 20);
    
    len *= 0.7f;
    if (len > 3) {
        ofPushMatrix();
        motif(-len);
        ofPopMatrix();
        
        ofPushMatrix();
        ofRotate(-len*6);
        motif(len);
        ofPopMatrix();
    }
}

