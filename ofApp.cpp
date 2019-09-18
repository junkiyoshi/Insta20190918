#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofRotate(90);
	ofClear(0);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 200, true, true, true);
	/* https://github.com/kazesawa/kazesawa */

	font.drawString("1006", font.stringWidth("1006") * -0.5, 0);
	font.drawString("Days", font.stringWidth("Days") * -0.5, font.stringHeight("Days") * 1);

	fbo.end();

	ofPixels pixels;
	fbo.readToPixels(pixels);
	auto span = 10;

	ofColor pix_color;
	for (int x = 0; x < fbo.getWidth(); x += span) {

		for (int y = 0; y < fbo.getHeight(); y += span) {

			pix_color = pixels.getColor(x, y);
			if (pix_color != ofColor(0, 0)) {

				for (int z = span * -1; z <= span * 1; z += span) {

					this->location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, y - ofGetHeight() * 0.5, z));
				}
			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> next_index = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				next_index.push_back(index);
			}
		}

		this->next_index_list.push_back(next_index);
	}

	for (int i = 0; i < 2000; i++) {

		this->actors.push_back(new Actor(this->location_list, this->next_index_list, this->destination_list));
	}
}


//--------------------------------------------------------------
void ofApp::update() {

	int frame_span = 15;
	int prev_index_size = 0;

	if (ofGetFrameNum() % frame_span == 0) {

		prev_index_size = this->destination_list.size();
	}

	for (auto actor : this->actors) {

		actor->update(frame_span, this->location_list, this->next_index_list, this->destination_list);
	}

	if (prev_index_size != 0) {

		this->destination_list.erase(this->destination_list.begin(), this->destination_list.begin() + prev_index_size);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotate(90);
	ofRotateX(180);

	int index = 0;
	for (auto actor : this->actors) {

		actor->draw();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}