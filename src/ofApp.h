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

	void onGifSaved(string &fileName);
private:

	ofImage image;
	ofShader_check shader;
	ofFbo fbo;

    ofVec2f anchor;
    ofVec2f size_image;

	ofxPanel gui;
	ofParameterGroup parameters;
	string lastFileName;
	ofParameter<ofVec2f> pos;
	ofParameter<float> scale;
	ofParameter<bool> edit;

    ofParameter<float> dcol;
	ofParameter<float> mult;

	ofParameter<float> size_box;

	ofXml settings;

	ofTrueTypeFont font;

	bool hasAltPress;
	ofVec2f dMouse;
	ofVec2f prevPos_image;
	ofVec2f mousePress;

	ofVec2f prevPos_point;
	ofVec2f point;
	ofVec2f u_point;
	//------------------------------
	int gifFrame;
	float lastGifTime;
	float gifDuration;
	bool hasCaptureFrame;
	ofxGifEncoder gifEncoder;
	ofVec2f gif_size;
	//------------------------------
	ofVideoPlayer 		movie;
    //-----------------------------
    ofVideoGrabber vidGrabber;
};
