#pragma once
//#include "ofxKinectCommonBridge.h"

#include "ofMain.h"
//#include "ofxFluid.h"

#include "ofxGui.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxPostGlitch.h"
#include "ofxDatGui.h"

// Leap Motion SDK
#include "Leap.h"
#pragma comment(lib, "Leap.lib")
using namespace Leap;



#define FRAMENUM 100 //ì«Ç›çûÇﬁâÊëúÇÃñáêî

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void viewSkeletons(int ,int ,double);
	void save_img();

//	ofxKinectCommonBridge kinect;
//	ofImage cap_img;

	//ofImage kinectImage;
	//ofImage depthImage;
	//unsigned char * 	video_buff;
	//ofTexture			videoTexture;
	//unsigned char * 	videoInverted;
	//int color_width  ;
	//int color_height ;
	//ofImage video_rgb;

	//unsigned char * 	depth_buff;
	////unsigned char * 	depth_Inv;
	//ofTexture			depthTexture;
	//ofImage depth_Inv;

	//unsigned char * 	video_cut;
	//ofTexture			videoTexture_cut;


	//ofxFluid fluid;
	//ofxFluid fluid2;

	int     width, height;
	int     depth_width, depth_height;
	ofVec2f oldM;
	ofVec2f oldM2;

	ofxPanel gui;
	double dissipation_p1;
	ofParameter<bool> bHide;
	ofParameter<bool> depth_draw;
	ofParameter<bool> Skeleton_draw;
	ofParameter<bool> buff_draw;
	ofParameter<bool> rgb_draw;
	ofParameter<bool> rgb_draw_cirle;
	ofParameter<bool> time_draw;
	ofParameter<bool> life_draw;
	ofParameter<bool> save_log;

	ofParameter<ofColor> hand_color;
	ofParameter<ofColor> bar_color1;
	ofParameter<ofColor> bar_color2;
	ofParameter<string> screenSize;

	ofxFloatSlider p1, p2, p3, p4, p5, p6, p7, p8,p9,p10,p11;
	ofxFloatSlider rr2, gg2, bb2;
	ofxFloatSlider rr, gg, bb;
	ofVec3f head, lHand, rHand;
	bool hasSkeleton;

	ofxFloatSlider ave_rate1;
	ofxFloatSlider ave_rate2;
	ofxFloatSlider ave_thresh;
	ofParameter<bool>	doFullScreen;
	void				setFullScreen(bool& _value) { ofSetFullscreen(_value); }


	ofImage     brushImg;
	ofImage     rhand_img;
	ofImage     lhand_img;

	ofFbo       maskFbo;
	ofFbo       fbo;

	ofShader    shader;

	bool        bBrushDown;


	double	ratioMin;
	ofImage myImage_A[FRAMENUM];
	ofImage myImage_B[FRAMENUM];
	int currentFrame;
	int max_frame;

	ofImage image_buff;
	void image_buff_clear();
	double pixel_ratio;
	ofImage outImage;

	int page_status;//0:A,1:B
	int A_millis;
	int Q_millis;

	double time_msec_Q;
	double time_msec_A;
	string typeStr;

	ofxTrueTypeFontUC verdana14;
	ofxTrueTypeFontUC font;
	ofxPostGlitch	myGlitch;
	ofFbo			myFbo;
	ofParameter<bool> Glitch_flg;


	long rhand_ave;
	long lhand_ave;
	long rhand_ave_long;
	long lhand_ave_long;

	long rHand_x_ave;
	long rHand_y_ave;
	long rHand_x_ave_long;
	long rHand_y_ave_long;

	long lHand_x_ave;
	long lHand_y_ave;
	long lHand_x_ave_long;
	long lHand_y_ave_long;

	ofxDatGuiValuePlotter* plotter1;
	ofxDatGuiValuePlotter* plotter2;
	vector<ofxDatGuiComponent*> components;
	ofxDatGuiComponent* component;



	// Leap Motion
	Leap::Controller leap;



};
