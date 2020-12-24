#include "ofApp.h"

using namespace ofxCv;

void ofApp::setup() {
#ifdef TARGET_OSX
	//ofSetDataPathRoot("../data/");
#endif
	
	ofSetVerticalSync(true);
	ofFbo::Settings settings;
	srcTracker.setup();
	srcTracker.setIterations(25);
	srcTracker.setAttempts(4);

	faces.allowExt("jpg");
	faces.allowExt("png");
	faces.listDir("faces");

	//load the vertex and fragment shaders
	shader.load("blend.vert", "blend.frag");
	//initialize quadric object
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	//
	move = 0.0;
	faceNum = 0;
	loadFace("0000.jpg");
	faceNum = 1;
	loadFace("0001.jpg");
	
	ofDisableArbTex();
	frame = 0;
	add = 1.0;
	
}

void ofApp::update() {

	if (frame == 0) {
		for (int i = 0; i < ovc[0].size(); i++) {
			delaunay[0].triangleMesh.setVertex(i, ovc[0][i]);
			delaunay[1].triangleMesh.setVertex(i, ovc[1][i]);
		}
	}
	vector<glm::vec3>vc0 = delaunay[0].triangleMesh.getVertices();
	vector<glm::vec3>vc1 = delaunay[1].triangleMesh.getVertices();
	
	for (int i = 0; i < vc0.size(); i++) {
		vc0[i].x = vc0[i].x + diff[i].x*add;
		vc0[i].y = vc0[i].y + diff[i].y*add;
		delaunay[0].triangleMesh.setVertex(i, vc0[i]);
		vc1[i].x = vc0[i].x + diff[i].x*add;
		vc1[i].y = vc0[i].y + diff[i].y*add;
		delaunay[1].triangleMesh.setVertex(i, vc1[i]);
	}
	if(add > 0.0)frame++;
	else frame--;
	if (frame > 100) {
		frame = 100;
		add = -1.0;
	}
	else if(frame < 0) {
		frame = 0;
		add = 1.0;
	}
	//cout << vc0[30].x << " " << vc0[30].y << std::endl;
}


void ofApp::draw() {
	
	    //ofEnableArbTex();
		glDisable(GL_DEPTH_TEST);  // inportant!
		ofImage image[2];
		for (int i = 0; i < 2; i++) {
			fbo.begin();
			src[i].bind();
			delaunay[i].triangleMesh.draw();
			src[i].unbind();
			fbo.end();
			// ofTexture -> ofImage
			ofTexture tex = fbo.getTexture();
			ofPixels pixels;
			tex.readToPixels(pixels);
			image[i].setFromPixels(pixels);
			//image[i].draw(ofGetWidth() / 2 - src[1].getWidth() / 2, 0);
			//image->save("face.jpg");
		}
	
	cam.begin();
	ofScale(1, -1, 1);
	shader.begin();
	shader.setUniformTexture("before", image[0], 1);
	shader.setUniformTexture("after", image[1], 2);
	//shader.setUniform1f("blendAlpha", sin(ofGetElapsedTimef()) / 2 + .5); //fade 
	shader.setUniform1f("blendAlpha", 1.0-frame*0.01); //fade 
	move += 0.01;
	if (move > 1.0) move = 0.0;
	glEnable(GL_DEPTH_TEST); // important!
	// gluSphere(quadric, 150, 400, 10);
	//glTranslated(-ofGetWidth() / 4, 50.0, 0.0);
	glTranslated(0.0, 20.0, 0.0);
	glRotated(45, 0, 0, 1);
	gluDisk(quadric, 0, 350, 4, 4);
	//ofRect(100, 100, 200, 300);
	shader.end();
	cam.end();
	
}

void ofApp::loadFace(string face) {
	//cout << face << std::endl;
	ofImage img;
	img.loadImage(face);
	ofImage rectimg;
	int len;
	
	len = (img.getWidth() > img.getHeight()) ? img.getWidth() : img.getHeight();
	fbo.allocate(len, len, GL_RGBA);
	rectimg.allocate(len, len, OF_IMAGE_COLOR);
	ofFbo ftmp;
	ftmp.allocate(len, len, GL_RGBA);
	ftmp.begin();
	 rectimg.draw(0, 0);
	 img.draw(len-img.getWidth()-((len-img.getWidth())/2), 0);
	ftmp.end();
	ofTexture tex = ftmp.getTexture();
	ofPixels pixels;
	tex.readToPixels(pixels);
	rectimg.setFromPixels(pixels);
	//image[i].draw(ofGetWidth() / 2 - src[1].getWidth() / 2, 0);
	rectimg.save("rectimg.jpg");
	src[faceNum].loadImage("rectimg.jpg");
	//src[faceNum].resize(ofGetWidth() / 2, ofGetHeight());
	srcTracker.update(toCv(src[faceNum]));
	srcPoints[faceNum] = srcTracker.getImagePoints();
	//cout << srcPoints[faceNum].size() << std::endl;
	
	delaunay[faceNum].reset();
	
	delaunay[faceNum].addPoint(ofPoint(0.0, 0.0));
	delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(0, 0));

	for (int i = 0; i < srcPoints[faceNum].size(); i += 1) {
		//int j = srcPoints[faceNum].size() - i - 1;
		int j = i;
		delaunay[faceNum].addPoint(ofVec2f(srcPoints[faceNum].at(j).x, srcPoints[faceNum].at(j).y));
		delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(srcPoints[faceNum].at(j).x, srcPoints[faceNum].at(j).y));
	}
	
	//int wi = src[faceNum].getWidth();
	//int hi = src[faceNum].getHeight();
	int wi = len;
	int hi = len;
	// add points for corner
	delaunay[faceNum].addPoint(ofPoint(wi, 0.0));
	delaunay[faceNum].addPoint(ofPoint(wi, hi));
	delaunay[faceNum].addPoint(ofPoint(0.0, hi));
	// add texcoord for corner
	delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(wi, 0));
	delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(wi, hi));
	delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(0, hi));
	// triangulate
	delaunay[faceNum].triangulate();
	// add dummy uv for display
	delaunay[faceNum].triangleMesh.addTexCoord(glm::vec2(0.0, 1.0));
	//
	ovc[faceNum] = delaunay[faceNum].triangleMesh.getVertices();
	
	if (faceNum == 1) {
		glm::vec3 pt;
		for (int i = 0; i < ovc[1].size(); i++) {

			pt.x = (ovc[1][i].x - ovc[0][i].x)*0.01f;
			pt.y = (ovc[1][i].y - ovc[0][i].y)*0.01f;
			diff.push_back(pt);

		}
	}

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

	int x = dragInfo.position.x;
	int y = dragInfo.position.y;
	//faceNum = (x < (ofGetWidth()/2)) ? 0 : 1;
	cout << (int)x << std::endl;
	//loadFace(dragInfo.files[0]);
}

void ofApp::keyPressed(int key) {
	/*
	switch (key) {
	case OF_KEY_UP:
		//
		break;
	case OF_KEY_DOWN:
		//
		break;
    }
	*/
	
}
