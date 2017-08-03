#pragma once

#include "ofMain.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofxGui.h"




class EditFile
{
	long m_last_edit;
	string file_name;
public:
	void init(const string& file){
		m_last_edit = -1;
		file_name = ofFilePath::getAbsolutePath(file, true);
		isEdit();
	}

	bool isEdit(){
		long t_last_Edit = GetLastEdit(file_name.c_str());
		if(t_last_Edit != m_last_edit){
			m_last_edit = t_last_Edit;
			return true;
		}
		return false;
	}

	static long GetLastEdit(const char* path_to_file){
		struct tm* clock;
		struct stat attrib;
		stat(path_to_file, &attrib);
#if 0
		time_t rawtime;
		time(&rawtime);
		clock = gmtime(&rawtime);
#endif
		clock = gmtime(&( attrib.st_mtime ));
		return  clock->tm_hour * 10000
			+ clock->tm_min * 100
			+ clock->tm_sec;
	}
};

class ofShader_check :public ofShader
{
	EditFile m_EditFile_v;
	EditFile m_EditFile_f;
	ofShader m_t_shader;
	std::string file_name;
public:
	virtual bool load(string shaderName){
		file_name = shaderName;
		bool ret = ofShader::load(file_name);
		//shaderName + ".vert", shaderName + ".frag");
		m_EditFile_v.init(file_name + ".vert");
		m_EditFile_f.init(file_name + ".frag");
		return ret;
	}

	void update(){
		bool isEdit = m_EditFile_v.isEdit() + m_EditFile_f.isEdit();
		if(isEdit && isLoaded()){
			m_t_shader.load(file_name);
			if(m_t_shader.isLoaded() && m_t_shader.checkProgramLinkStatus(m_t_shader.getProgram())){
				m_t_shader.unload();
				ofLogNotice("ofShader") << "\n" << "##########################\n" << "reload shader";
				ofShader::load(file_name);
			}
		}
	}
};

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

	ofImage m_image;
	ofShader_check m_shader;
	ofFbo m_fbo;

    ofVec2f m_anchor;
    ofVec2f m_size_image;

	ofxPanel m_gui;
	ofParameterGroup m_parameters;
	ofParameter<ofVec2f> m_pos;
	ofParameter<float> m_scale;
	ofParameter<bool> m_edit;

    ofParameter<float> m_dcol;

	ofXml m_settings;

	ofTrueTypeFont m_font;

	bool isAltPress;
	ofVec2f m_dMouse;
	ofVec2f m_prevPos_image;
	ofVec2f m_MousePress;

	ofVec2f m_prevPos_point;
	ofVec2f m_point;
};
