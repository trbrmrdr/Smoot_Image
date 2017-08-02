#include "ofMain.h"
#include "ofApp.h"

#define _PROGRAMMABLE_RENDERER
//========================================================================
int main(){
#ifdef _PROGRAMMABLE_RENDERER
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 1);
	settings.width = 1024;
	settings.height = 768;
	ofCreateWindow(settings);
#else
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
