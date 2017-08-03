#include "ofApp.h"

#define SETTING_FILE "settings.xml"
#define IMAGE_FILENAME "IMG_20170722_171059.jpg"

//#define IMAGE_FILENAME "IMG_20170722_171059.jpg"
//#define IMAGE_FILENAME "IMG_20170722_171044.jpg"

//#define IMAGE_FILENAME "Kawg-7Ip1lo.png"

//#define IMAGE_FILENAME "iI6L9sm9c78.jpg"
#define IMAGE_FILENAME "AfDOYDIxkd8.jpg"

#define VIDEO_FILENAME "VID_20170722_163702.mp4_"
//--------------------------------------------------------------
void ofApp::setup(){

	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}
	else{
		shader.load("shadersGL2/shader");
		DebugBreak();
	}

	hasAltPress = false;
	dMouse = ofVec2f(0);
	u_point = ofVec2f(.0);
	//-------------------------------
	parameters.setName("settings");
	parameters.add(pos.set("pos", ofVec2f(0), ofVec2f(-2000.), ofVec2f(2000.)));
	parameters.add(scale.set("scale", 1., .1, 3.));
	parameters.add(edit.set("isEdit", false));
	parameters.add(dcol.set("d_col", .5, .0, 1.));
	parameters.add(mult.set("mult", 1., .0, 1.));

	parameters.add(size_box.set("size_box", 1., 1., 100.));

	gui.setup(parameters);
	gui.loadFromFile(SETTING_FILE);

	font.loadFont(OF_TTF_SANS, 20, true, true);
	ofEnableAlphaBlending();
	ofEnableAntiAliasing();
	ofSetVerticalSync(true);

	//-------------------------------

	movie.load(VIDEO_FILENAME);
	movie.setLoopState(OF_LOOP_NORMAL);
	movie.play();
	movie.setSpeed(.25);

	if(movie.isLoaded()){
		size_image = ofVec2f(movie.getWidth(), movie.getHeight());
	}
	else if(image.loadImage(IMAGE_FILENAME)){
		size_image = ofVec2f(image.getWidth(), image.getHeight());

	}
	else{
		DebugBreak();
	}
	anchor = ofVec2f(.5);
	//image.setAnchorPercent(anchor.x, anchor.y);


	fbo.allocate(size_image.x, size_image.y, GL_RGBA);
	fbo.setAnchorPercent(anchor.x, anchor.y);

	//-------------------------------
	hasCaptureFrame = false;
	gif_size = size_image;// *.5;

	gifEncoder.setup(gif_size.x, gif_size.y, .60, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	ofSetFullscreen(true);
}

void ofApp::onGifSaved(string &fileName){
	cout << "gif saved as " << fileName << endl;
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
	movie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	//--------------
	fbo.begin();

	shader.begin();
	//shader.setUniformTexture("tex0", image.getTextureReference(), 1);
	shader.setUniform2f("u_resolution", size_image);
	ofVec2f t_u_point = point / size_image + anchor;
	if(!( t_u_point.x<.0 || t_u_point.x>1. || t_u_point.y<.0 || t_u_point.y>1. ))
		u_point = t_u_point;
	shader.setUniform2f("u_point", u_point);
	shader.setUniform1i("u_press", mousePress.x == -1 ? 0 : 1);
	shader.setUniform1f("u_dcol", dcol.get());
	shader.setUniform1f("u_time", (float) ofGetSystemTime() / 5000.f);
	shader.setUniform1f("u_mult", mult.get());
	shader.setUniform1f("u_size_box", size_box.get());

	if(image.isAllocated()){
		image.draw(0, 0);
	}
	else{
		//ofSetHexColor(0xFFFFFF);

		//shader.setUniformTexture("tex0", movie.getTextureReference(), 1);
		movie.draw(0, 0);
		//ofSetHexColor(0x000000);
	}

	shader.end();
	fbo.end();

	//--------------

	ofPushMatrix();

	ofTranslate(pos.get());
	ofScale(scale.get(), scale.get());

	//-----------------------------------
	ofSetColor(ofColor::white);
	ofDrawLine(ofVec2f(.0, -10000.f), ofVec2f(.0, +10000.f));
	ofDrawLine(ofVec2f(-10000.f, .0f), ofVec2f(+10000.f, .0f));

	//-----------------------------------
	fbo.draw(0, 0);
	//-----------------------------------
	if(hasAltPress){
		ofSetColor(ofColor::green);
		ofDrawCircle(point, 10);
	}

	ofPopMatrix();

	if(hasCaptureFrame){
		ofSetColor(ofColor::red);
		font.drawString("hasCapture enable :" + ofToString(gifFrame), ofGetWidth() - 300, 140);


#if 1
		ofPixels pixels;
		fbo.readToPixels(pixels);

		float t_currTime = ofGetSystemTimeMicros();
		if(lastGifTime < 0)
			gifDuration = .1;
		else
			gifDuration = ( t_currTime - lastGifTime ) / 100000.;
		gifDuration *= .25;
		gifDuration = gifDuration < .0 ? .1 : gifDuration;
		//gifDuration = .1f;
		cout << gifDuration << "\n";

		gifEncoder.addFrame(
			pixels.getData(),
			gif_size.x,
			gif_size.y,
			pixels.getBitsPerPixel(),
			gifDuration
			//.1f
		);
		gifFrame++;
		lastGifTime = ofGetSystemTimeMicros();
#else
		ofPixels t_pixels;
		fbo.readToPixels(t_pixels);
		ofImage t_image(t_pixels);
		t_image.resize(gif_size.x, gif_size.y);


		float t_currTime = ofGetSystemTimeMicros();
		if(lastGifTime < 0)
			gifDuration = .1;
		else
			gifDuration = ( t_currTime - lastGifTime ) / 100000.;
		gifDuration *= .5;
		gifDuration = gifDuration < .0 ? .1 : gifDuration;
		//gifDuration = .1f;
		cout << gifDuration << "\n";

		gifEncoder.addFrame(
			t_image.getPixels().getData(),
			gif_size.x,
			gif_size.y,
			t_image.getPixels().getBitsPerPixel(),
			gifDuration
			//.1f
		);
		gifFrame++;
		lastGifTime = ofGetSystemTimeMicros();
#endif
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
			if(!hasCaptureFrame){
				gifEncoder.reset();
				hasCaptureFrame = true;
				gifFrame = 0;
				lastGifTime = -1.;
			}
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
			gifEncoder.save(string(IMAGE_FILENAME) + "_test.gif");
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
