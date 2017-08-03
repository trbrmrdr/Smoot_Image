#include "ofApp.h"

#define SETTING_FILE "settings.xml"
//--------------------------------------------------------------
void ofApp::setup(){
	image.loadImage("background.png");
	anchor = ofVec2f(.5);
	image.setAnchorPercent(anchor.x, anchor.y);

	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}
	else{
		shader.load("shadersGL2/shader");
		DebugBreak();
	}

	size_image = ofVec2f(image.getWidth(), image.getHeight());
	//fbo.allocate(width, height);
	hasAltPress = false;
	dMouse = ofVec2f(0);

	parameters.setName("settings");
	parameters.add(pos.set("pos", ofVec2f(0), ofVec2f(-2000.), ofVec2f(2000.)));
	parameters.add(scale.set("scale", 1., .1, 3.));
	parameters.add(edit.set("isEdit", false));
	parameters.add(dcol.set("d_col", .5, .0, 1.));

	gui.setup(parameters);
	gui.loadFromFile(SETTING_FILE);

	font.loadFont(OF_TTF_SANS, 20, true, true);
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofSetVerticalSync(true);

	//-------------------------------
	hasCaptureFrame = false;
	gifEncoder.setup(size_image.x, size_image.y, .25, 256);
}

void ofApp::exit(){
	settings.serialize(parameters);
	settings.save(SETTING_FILE);

	gifEncoder.exit();
}

//--------------------------------------------------------------
void ofApp::update(){
	shader.update();
	image.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	//_____________
	//fbo.begin();
	ofPushMatrix();

	ofTranslate(pos.get());
	ofScale(scale.get(), scale.get());

	//-----------------------------------
	ofSetColor(ofColor::white);
	ofDrawLine(ofVec2f(.0, -10000.f), ofVec2f(.0, +10000.f));
	ofDrawLine(ofVec2f(-10000.f, .0f), ofVec2f(+10000.f, .0f));

	//-----------------------------------
	shader.begin();
	//shader.setUniform1f("mouseX", mouseX);
	//shader.setUniformTexture("tex0", image.getTextureReference(), 1);
	shader.setUniform2f("point", point / size_image + anchor);
	shader.setUniform1i("press", mousePress.x == -1 ? 0 : 1);
	shader.setUniform1f("dcol", dcol.get());
	shader.setUniform1f("time", (float) ofGetSystemTime() / 5000.f);


	image.draw(0, 0);

	//fbo.draw(0, 0);
	shader.end();
	//fbo.end();

	//-----------------------------------
	if(hasAltPress){
		ofSetColor(ofColor::green);
		ofDrawCircle(point, 10);
	}

	ofPopMatrix();

	if(hasCaptureFrame){
		ofSetColor(ofColor::red);
		font.drawString("hasCapture enable :" + ofToString((int) ofGetFrameRate()), ofGetWidth() - 300, 140);

		gifEncoder.addFrame(
			image.getPixels().getData(),
			image.getWidth(),
			image.getHeight(),
			image.getPixelsRef().getBitsPerPixel(),
			.1f
		);
	}

	//-----------------------------------
	//fbo.draw(width*.5 - mouseX, height*.5 - mouseY, width, height);

	gui.draw();
	ofSetColor(ofColor::green);
	font.drawString("fps :" + ofToString((int) ofGetFrameRate()), ofGetWidth() - 300, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case OF_KEY_ALT:
			hasAltPress = true;
			break;
		case ' ':
			hasCaptureFrame = true;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		case OF_KEY_ALT:
			hasAltPress = false;
			break;
		case ' ':
			hasCaptureFrame = false;
			break;
		case 's':
			cout << "start saving\n" << endl;
			gifEncoder.save("test.gif");
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	//point = ( ofVec2f(x, y) - pos.get() ) / scale.get();
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
	if(hasAltPress && edit.get()){
		scale.set(scale.get() + .1*scrollY);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(mousePress.x == -1) return;

	if(hasAltPress && edit.get()){
		dMouse = mousePress - ofVec2f(x, y) - prevPos_image;
		pos.set(ofVec2f(-1.) * dMouse);
	}
	else{
		point = ( ofVec2f(x, y) - pos.get() ) / scale.get();
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mousePress = ofVec2f(x, y);
	prevPos_image = pos.get();
	prevPos_point = point;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	mousePress.x = -1.f;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
