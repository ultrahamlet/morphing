#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ofxDelaunay.h"
#include "GL/glu.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void dragEvent(ofDragInfo dragInfo);
	void loadFace(string face);

	void keyPressed(int key);

	ofxFaceTrackerThreaded camTracker;
	//ofVideoGrabber cam;

	ofxFaceTracker srcTracker;
	ofImage src[2];
	vector<glm::vec2> srcPoints[2];
	int faceNum;

	ofDirectory faces;
	ofxDelaunay delaunay[2];
	ofShader shader;
	GLUquadricObj *quadric; //for gluSphere
	ofEasyCam cam;
	//float move;
	int frame;
	ofFbo fbo;
	vector<glm::vec3>ovc[2];
	vector<glm::vec3>diff;
	float add;


};