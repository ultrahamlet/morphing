#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	//cout << "Working directory: " << system("dir;");
	//group_gui.setup();										//‰Šú‰»ˆ—
	//ofAddListener(btn.my_event, this, &ofApp::my_callback);
	
	//group_gui.add(btn.setup("play", 100,20));
	circleButton.set(ofGetWidth()/2-8, ofGetHeight()-50);
	radius = 16;
	bCircleButton = false;

	//rectButton.set(200, 50, 10, 10);
	//bRectButton = false;

}

//--------------------------------------------------------------
void ofApp::update(){


	if (bCircleButton) {
		//do something
	}
	else {
		//or do sth else
	}

	if (bRectButton) {
		//do some other things
	}
	else {
		//or do sth else
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

 if(icImg[0].getWidth() > 0)icImg[0].draw(0,0);
 if(icImg[1].getWidth() > 0)icImg[1].draw(ofGetWidth()/2, 0);
 //group_gui.draw();
 if (bCircleButton)
	 ofSetColor(ofColor::dimGray);
  else
	 ofSetColor(ofColor::white);

 ofCircle(circleButton, radius);

 /*
 if (bRectButton)
	 ofSetColor(ofColor::sandyBrown);
 else
	 ofSetColor(ofColor::seaGreen);
 ofRect(rectButton);
 */
 

}


void ofApp::my_callback(float &val) {
	cout << val << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case OF_KEY_UP:
		//
		break;
	case OF_KEY_DOWN:
		//
		system("AutoFaceMorph;");
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	//bCircleButton = false;
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	if (circleButton.distance(ofPoint(x, y)) < radius) {
		//bCircleButton = !bCircleButton;

		bCircleButton = true;
	}

	//if (rectButton.inside(x, y)) {
	//	bRectButton = !bRectButton;
	//}
	

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(bCircleButton)system("AutoFaceMorph;");
	bCircleButton = false;
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

	//morphing = false;
	int x = dragInfo.position.x;
	int y = dragInfo.position.y;
	//faceNum = (x < (ofGetWidth()/2)) ? 0 : 1;
	ofImage image;
	
	image.load(dragInfo.files[0]);

	system(("jpegexiforient " + dragInfo.files[0] + " > " + ofToDataPath("tmp.txt")).c_str());
	int orientation = ofToInt(ofBufferFromFile("tmp.txt").getFirstLine());
	cout << "orientation: " << orientation << endl;

	float degree = 0.0;
	if (orientation == 8) degree = -90.0;
	if (orientation == 3) degree = 180.0;
	if (orientation == 6) degree = 90.0;
	if (orientation == 1) degree = 0.0;

	float wi = (float)image.getWidth();
	float hi = (float)image.getHeight();
	if (hi < wi) {

		ofFbo mfbo;
		//mfbo.allocate(hi, wi, GL_RGBA);
		mfbo.allocate(hi, wi, GL_RGBA);
		mfbo.begin();
		ofPushMatrix();
		ofTranslate(hi / 2, wi / 2, 0);//move pivot to centre
		//ofRotate(ofGetFrameNum() * .01, 0, 0, 1);//rotate from centre
		ofRotate(degree, 0, 0, 1);//rotate from centre
		//ofPushMatrix();
		ofTranslate(-wi / 2, -hi / 2);//move back by the centre offset
		//rotate
		image.draw(0, 0);
		//ofPopMatrix();
		//ofPopMatrix();
		mfbo.end();
		ofPixels pixels;
		ofImage rotImage;
		(mfbo.getTexture()).readToPixels(pixels);
		image.setFromPixels(pixels);
		//image.save("rotimage.jpg");

		hi = image.getHeight();
		wi = image.getWidth();
		//cout << wi << " " << hi << std::endl;
		/*
		if (wi < hi)
		{
			if (hi > 1000) image.resize((int)((1000.0 / hi)*wi), 1000);
		}
		else
		{
			if (wi > 1000) image.resize(1000, (int)((1000.0 / wi)*hi));
		}
		//wi = image.getWidth();
		//hi = image.getHeight();
		cout << wi << " " << hi << std::endl;
		*/
	}

	
	wi = image.getWidth();
	hi = image.getHeight();
	//cout << wi << " " << hi << std::endl;
	if (hi > 1000) image.resize((int)((1000.0 / hi)*wi), 1000);

	if (x < (ofGetWidth() / 2)) {
		image.save("0000.jpg");
		icImg[0] = image;
		icImg[0].resize(ofGetWidth() / 2, (((ofGetWidth() /2)/ icImg[0].getWidth())*icImg[0].getHeight()));
		//cout << iconImg[0].getWidth();
		//imgDropped[0] = true;
		//if (imgDropped[1]) morphing = true;
	}
	else {
		image.save("0001.jpg");
		icImg[1] = image;
		icImg[1].resize(ofGetWidth() / 2, (((ofGetWidth() / 2) / icImg[1].getWidth())*icImg[1].getHeight()));
		//..imgDropped[1] = true;
		//if (imgDropped[0]) morphing = true;
		//loadFace("0001.jpg");
	}
	

}
