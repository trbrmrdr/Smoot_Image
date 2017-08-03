#include "ofApp.h"

#define SETTING_FILE "settings.xml"
//--------------------------------------------------------------
void ofApp::setup() {
    m_image.loadImage("background.png");
    m_anchor = ofVec2f(.5);
    m_image.setAnchorPercent(m_anchor.x, m_anchor.y);

    if (ofIsGLProgrammableRenderer()) {
        m_shader.load("shadersGL3/shader");
    }
    else {
        m_shader.load("shadersGL2/shader");
        DebugBreak();
    }

    m_size_image = ofVec2f(m_image.getWidth(), m_image.getHeight());
    //m_fbo.allocate(m_width, m_height);
    isAltPress = false;
    m_dMouse = ofVec2f(0);

    m_parameters.setName("settings");
    m_parameters.add(m_pos.set("pos", ofVec2f(0), ofVec2f(-2000.), ofVec2f(2000.)));
    m_parameters.add(m_scale.set("scale", 1., .1, 3.));
    m_parameters.add(m_edit.set("isEdit", false));
    m_parameters.add(m_dcol.set("d_col", .5, .0, 1.));

    m_gui.setup(m_parameters);
    m_gui.loadFromFile(SETTING_FILE);

    m_font.loadFont(OF_TTF_SANS, 9, true, true);
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetVerticalSync(true);
}

void ofApp::exit() {
    m_settings.serialize(m_parameters);
    m_settings.save(SETTING_FILE);
}

//--------------------------------------------------------------
void ofApp::update() {
    m_shader.update();
    m_image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    //_____________
    //m_fbo.begin();
    ofPushMatrix();

    ofTranslate(m_pos.get());
    ofScale(m_scale.get(), m_scale.get());

    //-----------------------------------
    ofSetColor(ofColor::white);
    ofDrawLine(ofVec2f(.0, -10000.f), ofVec2f(.0, +10000.f));
    ofDrawLine(ofVec2f(-10000.f, .0f), ofVec2f(+10000.f, .0f));

    //-----------------------------------
    m_shader.begin();
    //m_shader.setUniform1f("mouseX", mouseX);
    //m_shader.setUniformTexture("tex0", m_image.getTextureReference(), 1);
    m_shader.setUniform2f("m_point", m_point / m_size_image + m_anchor);
    m_shader.setUniform1i("m_press", m_MousePress.x == -1 ? 0 : 1);
    m_shader.setUniform1f("m_dcol", m_dcol.get());
    m_shader.setUniform1f("m_time", (float) ofGetSystemTime() / 5000.f);

    m_image.draw(0, 0);

    //m_fbo.draw(0, 0);
    m_shader.end();
    //m_fbo.end();

    //-----------------------------------
    if (isAltPress) {
        ofSetColor(ofColor::green);
        ofDrawCircle(m_point, 10);
    }

    ofPopMatrix();
    //-----------------------------------
    //m_fbo.draw(m_width*.5 - mouseX, m_height*.5 - mouseY, m_width, m_height);


    m_gui.draw();
    m_font.drawString("fps :" + ofToString((int) ofGetFrameRate()), ofGetWidth() - 300, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == OF_KEY_ALT) {
        isAltPress = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == OF_KEY_ALT) {
        isAltPress = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    //m_point = ( ofVec2f(x, y) - m_pos.get() ) / m_scale.get();
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    if (isAltPress && m_edit.get()) {
        m_scale.set(m_scale.get() + .1*scrollY);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    if (m_MousePress.x == -1) return;

    if (isAltPress && m_edit.get()) {
        m_dMouse = m_MousePress - ofVec2f(x, y) - m_prevPos_image;
        m_pos.set(ofVec2f(-1.) * m_dMouse);
    }
    else {
        m_point = ( ofVec2f(x, y) - m_pos.get() ) / m_scale.get();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    m_MousePress = ofVec2f(x, y);
    m_prevPos_image = m_pos.get();
    m_prevPos_point = m_point;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    m_MousePress.x = -1.f;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
