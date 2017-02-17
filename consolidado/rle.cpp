#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <utility>

using namespace std;

typedef struct MyPixel {

	uByte red;
	uByte green;
	uByte blue;
	
} MyPixel;

//void rle(int**, int, int);

/*int main() {
	int **image = (int**)malloc(sizeof(int*) * 6);

	for (int i = 0; i < 6; i++) {
		image[i] = (int*)malloc(sizeof(int) * 4);
	}

	image[0][0] = 1;
	image[0][1] = 1;
	image[0][2] = 1;
	image[0][3] = 1;
	
	image[1][0] = 2;
	image[1][1] = 2;
	image[1][2] = 3;
	image[1][3] = 2;
	
	image[2][0] = 5;
	image[2][1] = 5;
	image[2][2] = 2;
	image[2][3] = 2;
	
	image[3][0] = 3;
	image[3][1] = 3;
	image[3][2] = 3;
	image[3][3] = 3;
	
	image[4][0] = 4;
	image[4][1] = 5;
	image[4][2] = 6;
	image[4][3] = 7;
	
	image[5][0] = 9;
	image[5][1] = 8;
	image[5][2] = 9;
	image[5][3] = 8;

	rle(image, 6, 4);

	return 0;
}*/

void rle(PixelLab* image) {
	
	vector<pair<short int, MyPixel> > vPixels[image->GetHeight()];

	uByte r, g, b;
	uByte rOld, gOld, bOld;

	pair<short int, MyPixel> pairPixel;

	for (int i = 0; i < image->GetHeight(); i++) {
		for (int j = 0; j < image->GetWidth(); j++) {

			image->GetRGB(i, j, r, g, b);

			if (r == rOld && g == gOld && b == bOld && j > 0) {
				vPixels[i].back().first += 1;
			} else {
				MyPixel myPixel;
				myPixel.red = r;
				myPixel.green = g;
				myPixel.blue = b;
				pairPixel = make_pair(1, myPixel);
				vPixels[i].push_back(pairPixel);
			}

			rOld = r;
			gOld = g;
			bOld = b;
		}
	}

	double sizeSequence = 0;

	for (int i = 0; i < image->GetHeight(); i++) {
		sizeSequence += (sizeof(short int) + sizeof(MyPixel)) * vPixels[i].size();
	}

	double sizeOriginal = image->GetHeight() * image->GetWidth() * sizeof(MyPixel);

	cout << "Tamanho da imagem original: " << (sizeOriginal / 1024.0) << " KB\n";
	cout << "Tamanho da imagem compactado: " << (sizeSequence / 1024.0) << " KB\n";
}
