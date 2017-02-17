#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <algorithm>

using namespace std;

/*void lzw(int**, int, int);

int main() {
	int **image = (int**)malloc(sizeof(int*) * 6);

	for (int i = 0; i < 6; i++) {
		image[i] = (int*)malloc(sizeof(int) * 4);
	}

	image[0][0] = 1;
	image[0][1] = 2;
	image[0][2] = 2;
	image[0][3] = 1;
	
	image[1][0] = 2;
	image[1][1] = 2;
	image[1][2] = 1;
	image[1][3] = 2;
	
	image[2][0] = 2;
	image[2][1] = 1;
	image[2][2] = 2;
	image[2][3] = 2;
	
	image[3][0] = 1;
	image[3][1] = 2;
	image[3][2] = 2;
	image[3][3] = 3;
	
	image[4][0] = 3;
	image[4][1] = 4;
	image[4][2] = 1;
	image[4][3] = 2;
	
	image[5][0] = 5;
	image[5][1] = 6;
	image[5][2] = 5;
	image[5][3] = 5;

	lzw(image, 6, 4);

	return 0;
}*/

void lzw(PixelLab* image, vector<string> dicR, vector<string> dicG, vector<string> dicB, 
	vector<short int> sequenceR, vector<short int> sequenceG, vector<short int> sequenceB) {
//void lzw(int **image, int lin, int col) {
	//vector<string> dicR;
	//vector<string> dicG;
	//vector<string> dicB;
	
	//vector<int> sequenceR;
	//vector<int> sequenceG;
	//vector<int> sequenceB;

	vector<string> pR;
	vector<string> cR;
	vector<string> pcR;

	vector<string> pG;
	vector<string> cG;
	vector<string> pcG;

	vector<string> pB;
	vector<string> cB;
	vector<string> pcB;

	uByte r, g, b;

	int pos;

	string next;

	/*for (int i = 0; i <= 255; i++) {
		dicR.push_back(to_string(i));
		dicG.push_back(to_string(i));
		dicB.push_back(to_string(i));
	}*/

	image->GetRGB(0, 0, r, g, b);
	//r = image[0][0];

	pR.push_back("");
	cR.push_back(to_string(r));
	pcR.push_back(to_string(r));

	pG.push_back("");
	cG.push_back(to_string(g));
	pcG.push_back(to_string(g));

	pB.push_back("");
	cB.push_back(to_string(b));
	pcB.push_back(to_string(b));

	for (int i = 0; i < image->GetHeight(); i++) {
		for (int j = 0; j < image->GetWidth(); j++) {

			if (i == 0 && j == 0) continue;

			image->GetRGB(i, j, r, g, b);
			//r = image[i][j];

			/* Frequencia Red*/
			if (pR.back() == "") {
				next = cR.back();
			} else {
				next = pR.back() + "_" + cR.back();
			}

			if (find(dicR.begin(), dicR.end(), next) == dicR.end()) {
				dicR.push_back(next);
				pos = find(dicR.begin(), dicR.end(), pR.back()) - dicR.begin();
				sequenceR.push_back(pos);
				pR.push_back(cR.back());
			} else {
				pR.push_back(pcR.back());
			}

			cR.push_back(to_string(r));
			pcR.push_back(pR.back() + "_" + cR.back());
			/* Frequencia Red*/

			/* Frequencia Green*/
			if (pG.back() == "") {
				next = cG.back();
			} else {
				next = pG.back() + "_" + cG.back();
			}

			if (find(dicG.begin(), dicG.end(), next) == dicG.end()) {
				dicG.push_back(next);
				pos = find(dicG.begin(), dicG.end(), pG.back()) - dicG.begin();
				sequenceG.push_back(pos);
				pG.push_back(cG.back());
			} else {
				pG.push_back(pcG.back());
			}

			cG.push_back(to_string(g));
			pcG.push_back(pG.back() + "_" + cG.back());
			/* Frequencia Green*/

			/* Frequencia Blue*/
			if (pB.back() == "") {
				next = cB.back();
			} else {
				next = pB.back() + "_" + cB.back();
			}

			if (find(dicB.begin(), dicB.end(), next) == dicB.end()) {
				dicB.push_back(next);
				pos = find(dicB.begin(), dicB.end(), pB.back()) - dicB.begin();
				sequenceB.push_back(pos);
				pB.push_back(cB.back());
			} else {
				pB.push_back(pcB.back());
			}

			cB.push_back(to_string(b));
			pcB.push_back(pB.back() + "_" + cB.back());
			/* Frequencia Blue*/
		}
	}

	next = pR.back() + "_" + cR.back();

	if (find(dicR.begin(), dicR.end(), next) == dicR.end()) {
		pos = find(dicR.begin(), dicR.end(), pR.back()) - dicR.begin();
		sequenceR.push_back(pos);
		pos = find(dicR.begin(), dicR.end(), cR.back()) - dicR.begin();
		sequenceR.push_back(pos);
	} else {
		pos = find(dicR.begin(), dicR.end(), pcR.back()) - dicR.begin();
		sequenceR.push_back(pos);
	}

	next = pG.back() + "_" + cG.back();

	if (find(dicG.begin(), dicG.end(), next) == dicG.end()) {
		pos = find(dicG.begin(), dicG.end(), pG.back()) - dicG.begin();
		sequenceG.push_back(pos);
		pos = find(dicG.begin(), dicG.end(), cG.back()) - dicG.begin();
		sequenceG.push_back(pos);
	} else {
		pos = find(dicG.begin(), dicG.end(), pcG.back()) - dicG.begin();
		sequenceG.push_back(pos);
	}

	next = pB.back() + "_" + cB.back();

	if (find(dicB.begin(), dicB.end(), next) == dicB.end()) {
		pos = find(dicB.begin(), dicB.end(), pB.back()) - dicB.begin();
		sequenceB.push_back(pos);
		pos = find(dicB.begin(), dicB.end(), cB.back()) - dicB.begin();
		sequenceB.push_back(pos);
	} else {
		pos = find(dicB.begin(), dicB.end(), pcB.back()) - dicB.begin();
		sequenceB.push_back(pos);
	}

	double sizeSequence = sizeof(short int) * (sequenceR.size() + sequenceG.size() + sequenceB.size());

	for (int i = 0; i < dicR.size(); i++) {
		sizeSequence += dicR[i].size();
	}

	for (int i = 0; i < dicG.size(); i++) {
		sizeSequence += dicG[i].size();
	}

	for (int i = 0; i < dicB.size(); i++) {
		sizeSequence += dicB[i].size();
	}

	double originalSize = image->GetHeight() *image->GetWidth() * sizeof(uByte) * 3;

	cout << "Tamanho da imagem original: " << (originalSize / 1024.0) << " KB\n";
	cout << "Tamanho da imagem compactada: " << (sizeSequence / 1024.0) << " KB\n";
}
