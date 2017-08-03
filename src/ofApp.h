#pragma once

#include "ofMain.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofShader_check.h"

#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp
{

public:
	virtual void setup();
	virtual void exit();
	virtual void update();
	virtual void draw();

	virtual void keyPressed(int key);
	virtual void keyReleased(int key);
	virtual void mouseMoved(int x, int y);
	virtual void mouseDragged(int x, int y, int button);
	virtual void mousePressed(int x, int y, int button);
	virtual void mouseReleased(int x, int y, int button);
	virtual void mouseScrolled(int x, int y, float scrollX, float scrollY);
	virtual void mouseEntered(int x, int y);
	virtual void mouseExited(int x, int y);
	virtual void windowResized(int w, int h);
	virtual void dragEvent(ofDragInfo dragInfo);
	virtual void gotMessage(ofMessage msg);

private:

	ofImage image;
	ofShader_check shader;
	ofFbo fbo;

    ofVec2f anchor;
    ofVec2f size_image;

	ofxPanel gui;
	ofParameterGroup parameters;
	ofParameter<ofVec2f> pos;
	ofParameter<float> scale;
	ofParameter<bool> edit;

    ofParameter<float> dcol;

	ofXml settings;

	ofTrueTypeFont font;

	bool hasAltPress;
	ofVec2f dMouse;
	ofVec2f prevPos_image;
	ofVec2f mousePress;

	ofVec2f prevPos_point;
	ofVec2f point;
	

	bool hasCaptureFrame;
	ofxGifEncoder gifEncoder;
};
