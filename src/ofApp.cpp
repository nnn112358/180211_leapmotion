#include "ofApp.h"
#include "Header.h"


void ofApp::image_buff_clear() {

	unsigned char *data = image_buff.getPixels();
	int ww = image_buff.getWidth();
	for (int y = 0; y < image_buff.getHeight(); y++) {
		for (int x = 0; x < image_buff.getWidth(); x++) {
			int val = 0;
			data[y *ww + x] = val;
		}
	}
	image_buff.update();

}

void ofApp::save_img() {
	//cap_img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	//cap_img.resize(640,360);
	//string fname1 = ".\\log\\"+ofGetTimestampString() + "_cap.jpg";
	//fluid.save(fname1, OF_IMAGE_QUALITY_BEST);

	//string fname2 = ".\\log\\" + ofGetTimestampString() + "_rgb.jpg";
	//video_rgb.save(fname2, OF_IMAGE_QUALITY_BEST);

}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	//ファイルの数
	long nn_max = 0;
	string fnamelist[1000];
	string dir, ff, ext;
	dir = ".\\data\\images\\";
	ext = "png";
	get_filelist(dir, fnamelist, nn_max, ext);
	page_status = 0;

	ofBackground(0, 0, 0);
	currentFrame = 0;
	max_frame = nn_max / 2;
	//連番がふられた画像を順番に読み込み
	for (int i = 0; i < max_frame; i++) {
		//ファイル名を一時的に格納する文字列
		char char1[1024];
		sprintf(char1, "images/image%03d_A.png", i + 1);
		myImage_A[i].loadImage(char1);
		cout << char1 << ":read" << endl;
		char char2[1024];
		sprintf(char2, "images/image%03d_B.png", i + 1);
		myImage_B[i].loadImage(char2);
		cout << char2 << ":read" << endl;
	}

	brushImg.load("brush.png");
	brushImg.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	rhand_img.loadImage("hand_r.png");
	lhand_img.loadImage("hand_l.png");
	rhand_img.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	lhand_img.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く


	int depth_width = 640;
	int depth_height = 480;
	//depth_Inv.allocate(depth_width, depth_height, OF_IMAGE_GRAYSCALE);
	image_buff.allocate(depth_width, depth_height, OF_IMAGE_GRAYSCALE);
	image_buff_clear();

	gui.setup("parametor");
	gui.add(screenSize.set("screenSize", ""));
	gui.add(bHide.set("guiHide(h)", false));

	gui.add(doFullScreen.set("fullscreen (F)", false));
	//gui.add(depth_draw.set("depth_draw", false));
	//gui.add(rgb_draw.set("color_draw", true));
	//gui.add(rgb_draw_cirle.set("rgb_draw_cirle", true));
	gui.add(buff_draw.set("buff_draw", false));
	//gui.add(Skeleton_draw.set("Skeleton_draw", false));

	gui.add(Glitch_flg.set("Glitch", false));
	gui.add(time_draw.set("time_draw", true));
	gui.add(life_draw.set("life_draw", true));
	gui.add(save_log.set("save_log", true));


	gui.add(p1.setup("Circle Size", 50, 0, 400));
	gui.add(p2.setup("Threshold", 0.7, 0, 1.0));
	gui.add(p4.setup("Quation Page_Time", 50.0, 0, 300.0));
	gui.add(p3.setup("Answer Page Time", 10.0, 0, 300.0));
	gui.add(ave_rate1.setup("ave_rate1", 0.50, 0.01, 0.50));
	gui.add(ave_rate2.setup("ave_rate2", 0.1, 0.01, 0.50));
	gui.add(ave_thresh.setup("ave_thresh", 100, 0, 320));

	gui.add(p5.setup("Font Size", 30.0, 10, 100.0));
	gui.add(p6.setup("BarScale", 0.6, 0.1, 1.0));
	gui.add(p7.setup("BarPos_x", 300, 0, 1000));
	gui.add(p9.setup("BarPos_h", 50, 0, 1000));
	gui.add(p10.setup("BarPos_y", 125, 0, 1600));
	gui.add(p8.setup("StringPos_y", 125, 0, 1600));

	gui.add(hand_color.set("Hand_Col", ofColor(252, 226, 196, 255), ofColor(0, 0), ofColor(255, 255)));
	gui.add(bar_color1.set("Bar1_Col", ofColor(255, 0, 0), ofColor(0, 0), ofColor(255, 255)));
	gui.add(bar_color2.set("Bar2_Col", ofColor(0, 255, 0), ofColor(0, 0), ofColor(255, 255)));


	doFullScreen.addListener(this, &ofApp::setFullScreen);
	//gui.add(p8.setup("py", 0, -50, +50));
	gui.loadFromFile("settings.xml");
	//kinect.initSensor();
	//kinect.initIRStream(640, 480);
	int color_width = 640;
	int color_height = 480;
	//kinect.initColorStream(color_width, color_height, true);
	//kinect.initDepthStream(depth_width, depth_height, true);
	//kinect.initSkeletonStream(true);
	////simple start
	//kinect.start();
	//ofDisableAlphaBlending(); //Kinect alpha channel is default 0;
	ofEnableAlphaBlending();
	ofSetCircleResolution(64);
	//hasSkeleton = true;
	maskFbo.allocate(1920, 1080);
	fbo.allocate(1920, 1080);

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/alphamask.vert", "shaders_gles/alphamask.frag");
#else
	if (ofIsGLProgrammableRenderer()) {
		string vertex = "#version 150\n\
    	\n\
		uniform mat4 projectionMatrix;\n\
		uniform mat4 modelViewMatrix;\n\
    	uniform mat4 modelViewProjectionMatrix;\n\
    	\n\
    	\n\
    	in vec4  position;\n\
    	in vec2  texcoord;\n\
    	\n\
    	out vec2 texCoordVarying;\n\
    	\n\
    	void main()\n\
    	{\n\
	        texCoordVarying = texcoord;\
    		gl_Position = modelViewProjectionMatrix * position;\n\
    	}";
		string fragment = "#version 150\n\
		\n\
		uniform sampler2DRect tex0;\
		uniform sampler2DRect maskTex;\
        in vec2 texCoordVarying;\n\
		\
        out vec4 fragColor;\n\
		void main (void){\
		vec2 pos = texCoordVarying;\
		\
		vec3 src = texture(tex0, pos).rgb;\
		float mask = texture(maskTex, pos).r;\
		\
		fragColor = vec4( src , mask);\
		}";
		shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
		shader.bindDefaults();
		shader.linkProgram();
	}
	else {
		string shaderProgram = "#version 120\n \
		#extension GL_ARB_texture_rectangle : enable\n \
		\
		uniform sampler2DRect tex0;\
		uniform sampler2DRect maskTex;\
		\
		void main (void){\
		vec2 pos = gl_TexCoord[0].st;\
		\
		vec3 src = texture2DRect(tex0, pos).rgb;\
		float mask = texture2DRect(maskTex, pos).r;\
		\
		gl_FragColor = vec4( src , mask);\
		}";
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
		shader.linkProgram();
	}
#endif

	// otherwise it will bring some junk with it from the memory    
	maskFbo.begin();
	ofClear(0, 0, 0, 255);
	maskFbo.end();

	fbo.begin();
	ofClear(255, 255, 255, 255);
	//ofClear(0, 0, 0, 255);
	fbo.end();

	bBrushDown = true;

	A_millis = ofGetElapsedTimeMillis();
	Q_millis = ofGetElapsedTimeMillis();

	verdana14.loadFont("HuiFont29.ttf", 14);

	font.loadFont("HuiFont29.ttf", p5);

	//video_rgb.allocate(color_width, color_height, OF_IMAGE_COLOR);
	myGlitch.setup(&fbo);

	rhand_ave = 0;
	lhand_ave = 0;
	rHand_x_ave = 0;
	rHand_y_ave = 0;
	rHand_x_ave_long = 0;
	rHand_y_ave_long = 0;
	int x = ofGetWidth() - 300;
	int y = 0;

	plotter1 = new ofxDatGuiValuePlotter("RightHand", 0, 640 / 4.0);
	plotter1->setSpeed(2.0f);
	plotter1->setDrawMode(ofxDatGuiGraph::LINES);
	plotter2 = new ofxDatGuiValuePlotter("LeftHand ", 0, 640 / 4.0);
	plotter2->setSpeed(2.0f);
	plotter2->setDrawMode(ofxDatGuiGraph::LINES);

	component = plotter1;
	component->setPosition(x, y);
	components.push_back(component);
	component = plotter2;
	component->setPosition(x, y + 80);
	components.push_back(component);
}

//--------------------------------------------------------------
void ofApp::update() {
	//kinect.update();

	//色情報の処理・RGBの抽出と反転
	//kinectImage = kinect.getColorPixelsRef();
	//unsigned char *pixels = kinectImage.getPixels();
	//unsigned char *pixels2 = video_rgb.getPixels();

	//for (int y = 0; y < video_rgb.getHeight(); y++) {
	//	for (int x = 0; x < video_rgb.getWidth(); x++) {
	//		int w = video_rgb.getWidth();
	//		int h = video_rgb.getHeight();
	//		//それぞれのピクセルのr, g, bを抽出
	//		pixels2[y * 3 * w + x * 3] = pixels[y * 4 * w + x * 4 + 2];			//赤
	//		pixels2[y * 3 * w + x * 3 + 1] = pixels[y * 4 * w + x * 4 + 1];	//緑
	//		pixels2[y * 3 * w + x * 3 + 2] = pixels[y * 4 * w + x * 4 + 0];	//青
	//	}
	//}
	//video_rgb.update();

	//depthImage = kinect.getDepthPixelsRef();

	//if (kinect.isNewSkeleton()) {
	//	for (int i = 0; i < kinect.getSkeletons().size(); i++) {
	//		if (kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD) != kinect.getSkeletons().at(i).end()) {
	//			// just get the first one
	//			SkeletonBone headBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HEAD)->second;
	//			SkeletonBone lHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_LEFT)->second;
	//			SkeletonBone rHandBone = kinect.getSkeletons().at(i).find(NUI_SKELETON_POSITION_HAND_RIGHT)->second;
	//			ofVec3f hb(headBone.getScreenPosition().x, headBone.getScreenPosition().y, 0);
	//			head = head.getInterpolated(hb, 0.5);
	//			head.z = ofInterpolateCosine(head.z, headBone.getStartPosition().x, 0.5) + 0.1;

	//			ofVec3f lhb(lHandBone.getScreenPosition().x, lHandBone.getScreenPosition().y, 0);
	//			lHand = lHand.getInterpolated(lhb, 0.5);
	//			lHand.z = ofInterpolateCosine(lHand.z, lHandBone.getStartPosition().x, 0.5);

	//			ofVec3f rhb(rHandBone.getScreenPosition().x, rHandBone.getScreenPosition().y, 0);
	//			rHand = rHand.getInterpolated(rhb, 0.5);
	//			rHand.z = ofInterpolateCosine(rHand.z, rHandBone.getStartPosition().x, 0.5);
	//		}
	//	}
	//}

	float power = 0;
	Leap::Frame frame = leap.frame();
	for (Leap::Hand hand : frame.hands()) {
		for (Leap::Finger finger : hand.fingers()) {
			if (finger.type() == Leap::Finger::TYPE_INDEX) {
				int x = ofMap(finger.tipPosition().x, -300, 300, 0, ofGetWidth());
				int y = ofMap(finger.tipPosition().y, 200, 600, ofGetHeight(), 0);
				power = ofMap(finger.tipPosition().z, 100, -100, 0, 100);
				if (hand.isRight() == 1) { 
					rHand.x = x;
					rHand.y = y;
				}
				if (hand.isLeft() == 1) {
					lHand.x = x;
					lHand.y = y;
				}
			}
		}
	}

	maskFbo.begin();
	if (bBrushDown) {
		if ((rHand.x != 0) && (rHand.y != 0)) {
			//double ratio_x = myImage_B[currentFrame].getWidth() /depth_width;
			//double ratio_y = myImage_B[currentFrame].getHeight() /depth_height;
			double ratio_x = 1.0;
			double ratio_y = 1.0;
			int circle_x = rHand.x*ratio_x;
			int circle_y = rHand.y*ratio_y;
			//if(page_status==0)
			brushImg.draw(circle_x, circle_y, p1, p1);

			int circle_x2 = lHand.x*ratio_x;
			int circle_y2 = lHand.y*ratio_y;
			//if(page_status == 0)
			brushImg.draw(circle_x2, circle_y2, p1, p1);
		}
	}
	maskFbo.end();

	fbo.begin();
	ofClear(0, 0, 0, 0);
	shader.begin();
	shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1);
	double imgW = myImage_B[currentFrame].getWidth();
	double imgH = myImage_B[currentFrame].getHeight();
	myImage_B[currentFrame].draw(0, 0, ratioMin*imgW, ratioMin*imgH);

	shader.end();
	fbo.end();

	static int currentFrame_old = currentFrame;

	if (currentFrame_old == currentFrame) {
		unsigned char *data_buff = image_buff.getPixels();
		int circle_r = p1 / 2.0 / 3.0;

		for (int y = 0; y < image_buff.getHeight(); y++) {
			for (int x = 0; x < image_buff.getWidth(); x++) {
				int imgW = image_buff.getWidth();
				//int depth_imgW = depth_Inv.getWidth();

				int circle_x = rHand.x - x;
				int circle_y = rHand.y - y;
				if ((circle_x*circle_x + circle_y*circle_y < circle_r*circle_r) && (rHand.x != 0) && (rHand.y != 0)) {
					int val = 255;
					data_buff[y * imgW + x] = val;
				}

				int circle_x2 = lHand.x - x;
				int circle_y2 = lHand.y - y;

				if ((circle_x2*circle_x2 + circle_y2*circle_y2 < circle_r*circle_r) && (lHand.x != 0) && (lHand.y != 0)) {
					int val = 255;
					data_buff[y * imgW + x] = val;
				}

			}
		}
		image_buff.update();
		currentFrame_old = currentFrame;
	}
	else {
		image_buff_clear();
		currentFrame_old = currentFrame;
		pixel_ratio = 0;
		Q_millis = ofGetElapsedTimeMillis();
	}

	int pixel_cnt = 0;
	unsigned char *data_buff = image_buff.getPixels();
	for (int y = 0; y < image_buff.getHeight(); y++) {
		for (int x = 0; x < image_buff.getWidth(); x++) {
			int imgW = image_buff.getWidth();
			if (data_buff[y * imgW + x] == 255) pixel_cnt++;

		}
	}

	//if (page_status == 0)
	pixel_ratio = (double)pixel_cnt / (image_buff.getWidth()* image_buff.getHeight());

	char buf[100];
	sprintf(buf, "%lf", pixel_ratio);
	ofDrawBitmapString(buf, 20, 20);

	//ある程度消せている
	if (pixel_ratio > p2) {
		if (page_status == 0) {
		page_status = 1;
		if(save_log)save_img();
		A_millis = ofGetElapsedTimeMillis();
		image_buff_clear();
		}
	}


	if (page_status == 1) {	//答えのページへ遷移
		int n_millis = ofGetElapsedTimeMillis();
		time_msec_A = n_millis - A_millis;

		if (n_millis - A_millis < p3 * 1000) {//ある程度回答を見せる。
			maskFbo.begin();
			ofClear(255, 255, 255, 255);
			maskFbo.end();
		}

		else {									//次の問題へ行く。

			currentFrame++;
			if (currentFrame >= max_frame) currentFrame = 0;
			maskFbo.begin();
			ofClear(0, 0, 0, 255);
			maskFbo.end();
			page_status = 0;
		}
	}


	//問題をずっと出している
	int n_millis = ofGetElapsedTimeMillis();
	time_msec_Q = n_millis - Q_millis;
	if (n_millis - Q_millis > p4 * 1000) {
		Q_millis = ofGetElapsedTimeMillis();

		maskFbo.begin();
		ofClear(0, 0, 0, 255);
		maskFbo.end();

		image_buff_clear();

		currentFrame++;
		if (currentFrame >= max_frame)currentFrame = 0;
	}


	double rate1 = ave_rate1;
	double rate2 = ave_rate2;
	rHand_x_ave = rate1*rHand.x + (1 - rate1)*rHand_x_ave;
	rHand_x_ave_long = rate2* rHand.x + (1 - rate2)*rHand_x_ave_long;
	rHand_y_ave = rate1*rHand.y + (1 - rate1)*rHand_y_ave;
	rHand_y_ave_long = rate2* rHand.y + (1 - rate2)*rHand_y_ave_long;


	lHand_x_ave = rate1*lHand.x + (1 - rate1)*lHand_x_ave;
	lHand_x_ave_long = rate2* lHand.x + (1 - rate2)*lHand_x_ave_long;
	lHand_y_ave = rate1*lHand.y + (1 - rate1)*lHand_y_ave;
	lHand_y_ave_long = rate2* lHand.y + (1 - rate2)*lHand_y_ave_long;

	rhand_ave = 1.0*sqrt((rHand_x_ave - rHand_x_ave_long)*(rHand_x_ave - rHand_x_ave_long) + (rHand_y_ave - rHand_y_ave_long)*(rHand_y_ave - rHand_y_ave_long));
	lhand_ave = 1.0*sqrt((lHand_x_ave - lHand_x_ave_long)*(lHand_x_ave - lHand_x_ave_long) + (lHand_y_ave - lHand_y_ave_long)*(lHand_y_ave - lHand_y_ave_long));

	if ((rhand_ave + lhand_ave) > ave_thresh) {
		if (page_status == 1)	myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
	}
	else myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);

	plotter1->setValue(rhand_ave);
	plotter2->setValue(lhand_ave);

	for (int i = 0; i < components.size(); i++) components[i]->update();

	int xx = ofGetWidth() - 500;
	int yy = 0;
	double speed = 2;
	double max = 1000;
	double min = 0;
	components[0]->setPosition(xx, yy);
	components[1]->setPosition(xx, yy + 80);



}







void ofApp::draw() {

	ofSetColor(255, 255, 255, 255);


	float winW = ofGetWidth();
	float winH = ofGetHeight();
	float imgW = myImage_A[currentFrame].getWidth();
	float imgH = myImage_A[currentFrame].getHeight();

	float ratioW = winW / imgW;
	float ratioH = winH / imgH;

	if (ratioW < ratioH)	ratioMin = ratioW;
	else		ratioMin = ratioH;


	myImage_A[currentFrame].draw(0, 0, imgW*ratioMin, imgH*ratioMin);

	if (Glitch_flg)myGlitch.generateFx();
	fbo.draw(0, 0);


	//unsigned char *data = depthImage.getPixels();
	//unsigned char *data2 = depth_Inv.getPixels();
	//  make it negative
	//int bytes = depthImage.getWidth() * depthImage.getHeight();
	//for (int k = 0; k < bytes; k++)
	//	data2[k] = 255 - data[k];
	//depth_Inv.update();

	//int width_D = 320;
	//int height_D = 240;
	//if (rgb_draw == true) {
	//	video_rgb.draw(winW - width_D, winH - height_D, width_D, height_D);

	//	double ratio_x = width_D / (double)depth_width;
	//	double ratio_y = height_D / (double)depth_height;

	//	int circle_x = rHand.x*ratio_x + winW - width_D;
	//	int circle_y = rHand.y*ratio_y + winH - height_D;
	//	int circle_x2 = lHand.x*ratio_x + winW - width_D;
	//	int circle_y2 = lHand.y*ratio_y + winH - height_D;
	//	ofSetColor(255, 0, 0, 100);
	//	if(rgb_draw_cirle){	if ((rHand.x != 0) && (rHand.y != 0))	ofCircle(circle_x, circle_y, 20);
	//	if ((lHand.x != 0) && (lHand.y != 0))ofCircle(circle_x2, circle_y2, 20);
	//	}
	//	if (Skeleton_draw)	viewSkeletons(winW - width_D, winH - 1 * height_D, 0.5);
	//	ofSetColor(255, 255, 255, 255);

	//}


	//if (depth_draw == true) {
	//	depth_Inv.draw(winW - width_D, winH - 2 * height_D, width_D, height_D);

	//	double ratio_x = width_D / (double)depth_width;
	//	double ratio_y = height_D / (double)depth_height;

	//	int circle_x = rHand.x*ratio_x + winW - width_D;
	//	int circle_y = rHand.y*ratio_y + winH - 2 * height_D;
	//	int circle_x2 = lHand.x*ratio_x + winW - width_D;
	//	int circle_y2 = lHand.y*ratio_y + winH - 2 * height_D;
	//	ofSetColor(255, 0, 0, 100);
	//	if ((rHand.x != 0) && (rHand.y != 0)) ofCircle(circle_x, circle_y, 20);
	//	if ((lHand.x != 0) && (lHand.y != 0)) ofCircle(circle_x2, circle_y2, 20);
	//	ofSetColor(255, 255, 255, 255);

	//	if (Skeleton_draw)	viewSkeletons(winW - width_D, winH - 2 * height_D, 0.5);

	//}

	if (buff_draw) {
		image_buff.draw(winW - 640, winH - 480, 640, 480);
		if (Skeleton_draw)	viewSkeletons(winW - 640, winH - 3 * 480, 0.5);

	}



	double time_left = 0;
	double time_left_rate = 0;

	if (page_status == 0) {
		ofSetColor(225);
		time_left = p4 - time_msec_Q / 1000.0;
		time_left_rate = time_left / p4;

	}
	if (page_status == 1) {
		ofSetColor(225);
		time_left = p3 - time_msec_A / 1000.0;
		time_left_rate = time_left / p3;
	}


	if (!bHide) {
		gui.draw();
		char moji[4096];
		if (page_status == 0) {
			sprintf(moji, "Next question  %.1lf sec", time_left);
			verdana14.drawString(moji, 20, 35);
		}
		else {
			sprintf(moji, "Next question %.1lf sec", time_left);
			verdana14.drawString(moji, 20, 35);
		}
		sprintf(moji, "Eraser %.1lf %%", pixel_ratio*100.0);
		verdana14.drawString(moji, 20, 60);

		ofSetColor(255, 255, 255, 0);
		for (int i = 0; i < components.size(); i++) components[i]->draw();
	}


	//double ratio_x = ratioMin*imgW / depth_width;
	//double ratio_y = ratioMin*imgH / depth_height;
	double ratio_x = 1.0;
	double ratio_y = 1.0;

	int circle_x = rHand.x*ratio_x;
	int circle_y = rHand.y*ratio_y;
	int circle_x2 = lHand.x*ratio_x;
	int circle_y2 = lHand.y*ratio_y;
	//ofSetColor(252, 226, 196, 255);
	ofSetColor(hand_color);
	
	if ((circle_x != 0) && (circle_y != 0))		rhand_img.draw(circle_x, circle_y, p1 / 1.0, p1 / 1.0);
	if ((circle_x2 != 0) && (circle_y2 != 0))	lhand_img.draw(circle_x2, circle_y2, p1 / 1.0, p1 / 1.0);


	if (life_draw) {
		double pixel_ratio_view = winW*(p2 - pixel_ratio) / (p2)*p6;
		if (pixel_ratio_view < 0) pixel_ratio_view = 0;
		ofSetColor(bar_color1);
		font.drawString("きえたりょう", 0, winH - p8);
		ofRect(p7, winH - p10 - p9 / 2.0, pixel_ratio_view, p9);
	}
	if (time_draw) {
		double time_status_view = winW*time_left_rate*p6;
		ofSetColor(bar_color2);
		font.drawString("のこりじかん", 0, winH - p8 + p9);
		ofRect(p7, winH - p10 + p9 - p9 / 2.0, time_status_view, p9);
	}

	ofSetColor(255, 255, 255, 255);



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	//maskFbo.begin();
	//ofClear(0, 0, 0, 255);
	//maskFbo.end();


	if (key == 'h') {
		bHide = !bHide;
	}
	else if (key == 's') {
		gui.saveToFile("settings.xml");
	}
	else if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}


	else if (key == 'f') {
		doFullScreen.set(!doFullScreen.get());
	}

	else if (key == 'x') {
		currentFrame++;
		if (currentFrame >= max_frame)currentFrame = 0;
		page_status = 0;

	}
	else if (key == 'c') {
		maskFbo.begin();
		ofClear(255, 255, 255, 255);
		maskFbo.end();
		//page_status = 1;
	}
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);

	if (key == 'z') {
		save_img();
	}
	

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {


	static int flg = 5;
	if (flg == 0) {
		image_buff_clear();

		maskFbo.begin();
		ofClear(255, 255, 255, 255);

		maskFbo.end();
		flg = 1;
	}
	else if (flg == 1) {
		image_buff_clear();

		maskFbo.begin();
		ofClear(0, 0, 0, 255);

		maskFbo.end();
		currentFrame++;
		if (currentFrame >= max_frame)currentFrame = 0;
		flg = 0;
	}
	Sleep(50);

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	screenSize = ofToString(w) + "x" + ofToString(h);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
//--------------------------------------------------------------
void ofApp::viewSkeletons(int xx, int yy, double scale) {

	//ofPushStyle();
	//ofSetColor(255, 0, 0);
	//ofSetLineWidth(3.0f);
	//auto skeletons = kinect.getSkeletons();

	////double ratio_x = ofGetWidth() / depth_width;
	////double ratio_y = ofGetHeight() / depth_height;
	//for (auto & skeleton : skeletons) {
	//	for (auto & bone : skeleton) {
	//		ofSetColor(0, 0, 255, 100);
	//		switch (bone.second.getTrackingState()) {
	//		case SkeletonBone::Inferred:
	//			ofSetColor(0, 0, 255);
	//			break;
	//		case SkeletonBone::Tracked:
	//			ofSetColor(0, 255, 0);
	//			break;
	//		case SkeletonBone::NotTracked:
	//			ofSetColor(255, 0, 0);
	//			break;
	//		}

	//		auto index = bone.second.getStartJoint();
	//		auto connectedTo = skeleton.find((_NUI_SKELETON_POSITION_INDEX)index);
	//		if (connectedTo != skeleton.end()) {
	//			//	if (index != 1) {
	//		//	ofLine(connectedTo->second.getScreenPosition(), bone.second.getScreenPosition());
	//			//	}
	//		}

	//		ofCircle(bone.second.getScreenPosition().x*scale + xx, bone.second.getScreenPosition().y*scale + yy, 10.0f);
	//	}
	//}
	//ofPopStyle();
}
