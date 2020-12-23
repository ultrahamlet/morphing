#include "ofApp.h"

int main() {
	ofSetupOpenGL(960, 480, OF_WINDOW);
	//ofGLFWWindowSettings settings;
	//settings.setGLVersion(3, 2);
	//settings.width = 1024;
	//settings.height = 768;
	//ofCreateWindow(settings);
	ofRunApp(new ofApp());
}