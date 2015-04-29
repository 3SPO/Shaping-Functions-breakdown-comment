//This code is originally written by andreas muller and has been seperated and modified for better understanding by Alex Esposito

#include <ofApp.h>

    // -----------------------------------------------------------------------
void ofApp::setup()
    {
        //this creates the foundation for the code
        ofSetFrameRate( 60 );
        screenLengthInSecs = 4.0f;
        autoPlay = true;
        keyTimeVel = 0;
        time = 0;
    }
    
    // -----------------------------------------------------------------------
void ofApp::update()
    {
        //The code for key presses right and left move the ball across the waveform at your own pace
        if( ofGetKeyPressed( OF_KEY_LEFT ) )
        {
            keyTimeVel = ofLerp( keyTimeVel, screenLengthInSecs * -0.02f, 0.02 );
            autoPlay = false;
        }
        else if ( ofGetKeyPressed( OF_KEY_RIGHT ) )
        {
            keyTimeVel = ofLerp( keyTimeVel, screenLengthInSecs *  0.02, 0.02 );
            autoPlay = false;
        }
        
        //This allows the ball to move without any key presses
        if( autoPlay )
        {
            time += ofGetLastFrameTime();
        }
        else
        {
            time += keyTimeVel;
        }
        
        keyTimeVel = ofLerp( keyTimeVel, 0.0f, 0.05 );
    }
    
    // -----------------------------------------------------------------------
void ofApp::draw()
    {
        //this is where the meat of the visible coding is done
        //first setting up the required variables
        float w = ofGetWidth();
        float h = ofGetHeight();
        float middleY = h * 0.7;
        float magnitude = 300;
        //creating a background for the terrain to exist
        ofRectangle terrainRect( 0, middleY-magnitude, w, magnitude );
        
        //remapping these variables allow the entirety of the window to be mapped to 0 to 1
        float mx = ofMap( ofGetMouseX(), 0, ofGetWidth(),  0.0f, 1.0f );
        float my = ofMap( ofGetMouseY(), 0, ofGetHeight(), 0.0f, 1.0f );
        
        //sets the gradient and blend mode for better aesthetic
        ofBackgroundGradient( ofColor( 10, 10, 40), ofColor::black, OF_GRADIENT_CIRCULAR );
        ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        
        //creates the line and sets in to a white color
        ofSetColor( ofColor::white );
        ofLine( ofVec2f( 0, middleY), ofVec2f( ofGetWidth(), middleY ) );	// draw a line showing the middle
        
        //holding the space bar allows for the background and line to disappear as to only see the movement of the ball
        bool drawClean = ofGetKeyPressed(' ');
        
        if( !drawClean )
        {
            // Draw some lines to show where the unit edges are (0,1,2, etc)
            drawDashes( time, (int)screenLengthInSecs, w / screenLengthInSecs );
            drawTerrain( terrainRect, ofGetWidth(), time, screenLengthInSecs );
        }
        
        // I'm storing this in a ofVec3f instead of a ofVec2f only because the oF ofVec2f doesn't have a cross product operator
        ofVec2f terrainMousePos 	= getTerrain( ofGetMouseX(), terrainRect, time, screenLengthInSecs );
        
        // We can easily get a normal for the point by simply sampling our terrain a little bit ahead in time and taking the cross product.
        // The further ahead we look, the smoother our normals will be, but look ahead too far and the normal isn't very representative
        float lookAheadPixels = 4;
        ofVec2f terrainMouseNormal = (terrainMousePos - getTerrain( ofGetMouseX() + lookAheadPixels, terrainRect, time, screenLengthInSecs)).getNormalized().getRotated(90);
        
        //creates the ball and makes it pink
        ofSetColor( ofColor::lightPink );
        ofCircle( terrainMousePos, 6.0 );
        
        //of drawclean isnt true draw the line
        if( !drawClean ) ofLine( terrainMousePos, terrainMousePos + (terrainMouseNormal*12));
        
        //sets color and fill for background
        ofSetColor( ofColor::slateGray, 20 );
        ofNoFill();
        ofRect( terrainRect );
        ofFill();
    }
    
    // -----------------------------------------------------------------------
float ofApp::getTerrainHeight( float _t )
    {
        //These are a large number of mathmatic algorithms that change the line based on a number of variables all contained in the MathUtils class. Furthermore by commenting and uncommenting certain "result" code lines you can get different effects in the code
        // We've set things up so the time units are seconds for this function
        float result = 0.0f;
        
        //result += shape::pulseSquare( 6.0, 1.0, _t );
        //result += shape::pulseTriangle( 3.0, 1.0, _t );
        //result += shape::pulseTriangle( 1.0, 1.0, _t );
        //result += shape::pulseLineDownUp( 3.0, 1.0, _t );
        //result += shape::pulseLineUpDown( 3.0, 1.0, _t );
        //result += shape::pulseSine( 3.0, 0.5, _t );
        //result += shape::pulseSmoothStep( 3.0, 0.0, 0.3, 0.8, 1.4, _t );
        //result += shape::pulseTriangle( 3.0, 1.5, _t );
        //result += shape::pulseLinearStep( 3.0, 0.0, 1.0, 2.0, 3.0, _t );
        //result += shape::pulseSmoothStep( 3.0, 0.0, 1.0, 1.2, 2.4, _t );
        //result += shape::pulseTriangle( 0.5, 0.5, _t ) * 0.2;
        result += shape::pulseSine( 0.5, 0.5, _t * (ofMap(ofNoise( _t * 0.3 ), 0, 1, 0.2, 1.0)) ) * 0.2;
        //result += shape::pulseSawTooth( 3.0, 1.0, _t );
        //result += ofNoise( _t * (16.0 * ofNoise( _t * 0.1 )) );// * shape::pulseSmoothStep( 3.0, 0.0, 0.3, 0.8, 1.4, _t );
        //result += ofNoise( _t * 1.0 ) * shape::pulseSmoothStep( 10.0, 0.0, 1.0, 3.0, 4.5, _t );
        //result *= ofNoise( _t * 0.1 );
        
        //result += ofNoise( _t * ((shape::pulseTriangle( 1.0, 1.0, _t ) * 2) + 2) );
        
        //result += ofNoise( _t * ofMap( shape::pulseTriangle( 10.0, 10.0, _t ), 0, 1, 0.01, 1) );
        //result += shape::fbm( _t * 1.4, 5 );
        //result += ofNoise( _t * 2 );
        
        //result += (shape::pulseTriangle( 10.0, 10.0, _t ) * 0.8) + (ofNoise( _t * 2 ) * 0.2);
        
        //result *= ofNoise( _t * 0.03 );
        
        //result /= 2.0;
        
        return result;
    }
    
    // -----------------------------------------------------------------------
ofVec2f ofApp::getTerrain( float _x, ofRectangle _rect, float _time, float _rectWidthInSecs )
    {
        //This is a function used to get the terrain in order to use this specific data
        float vertexTime = ofMap( _x, 0, _rect.width, _time, _time + _rectWidthInSecs );
        
        ofVec2f pos;
        pos.x = _x;
        pos.y = -getTerrainHeight( vertexTime ) * _rect.height;
        pos += _rect.position + ofVec2f(0, _rect.height);
        
        return pos;
    }
    
    // -----------------------------------------------------------------------
void ofApp::drawTerrain( ofRectangle _rect, int _res, float _time, float _screenLengthInSecs  )
    {
        //This is a function to actually draw the terrain
        float w = ofGetWidth();
        
        //sets the mesh
        ofMesh mesh;
        mesh.setMode( OF_PRIMITIVE_LINE_STRIP );
        
        for( int i = 0; i < _res; i++ )
        {
            float frac = i / (float)_res;
            float vertexTime = ofMap( i, 0, _res, _time, _time + _screenLengthInSecs );
            
            ofVec2f pos;
            pos.x = ofMap( i,  0, _res,  0, _rect.width );
            pos.y = -getTerrainHeight( vertexTime ) * _rect.height;
            
            pos += _rect.position + ofVec2f(0, _rect.height);// + pos;
            
            mesh.addVertex( pos );
        }
        ofSetColor( ofColor::white );
        mesh.draw();
    }
    
    
    // -----------------------------------------------------------------------
void ofApp::drawDashes( float _time, int _numDashes, float _pixelsPerSec )
    {
        //This draws the dashes in between the intervals
        float w = ofGetWidth();
        float h = ofGetHeight();
        ofMesh bgSecondsMesh;
        bgSecondsMesh.setMode( OF_PRIMITIVE_LINES );
        for( int i = 0; i < _numDashes; i++ )
        {
            float tmpX = w - ((fmodf( time, 1.0 ) + i) * _pixelsPerSec);
            bgSecondsMesh.addVertex( ofVec2f( tmpX, 0) );
            bgSecondsMesh.addVertex( ofVec2f( tmpX, h) );
        }
        ofSetColor( ofColor::white, 100 );
        bgSecondsMesh.draw();
    }
    
    // -----------------------------------------------------------------------
void ofApp::keyPressed( int _key )
    {
        //this will reset autoplay after being taken over by the key functions
        if( _key == 'a' )
        {
            autoPlay = true;
        }
    }
