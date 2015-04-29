//This code is originally written by andreas muller and has been seperated and modified for better understanding by Alex Esposito

#pragma once

#include "ofMain.h"
#include "Math/MathUtils.h"

typedef MathUtils shape; // Shortcut to make the code a tad more redable later

class ofApp : public ofBaseApp
{
	public:
        void setup() ;
        void update() ;
        void draw() ;
        float getTerrainHeight(float _t) ;
        ofVec2f getTerrain(float _x, ofRectangle _rect, float _time, float _rectWidthInSecs) ;
        void drawTerrain(ofRectangle _rect, int _res, float _time, float _screenLengthInSecs) ;
        void drawDashes(float _time, int _numDashes, float _pixelsPerSec) ;
        void keyPressed(int _key) ;
    
        // create all necessary variables
        float time;
        float screenLengthInSecs;
        float keyTimeVel;
        bool autoPlay;
    
};

    
        