#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>

void setNameToBuffer(char *name, char *buffer, int len) {
	int i = 0;
	for (i = 0; i < len; ++i) {
		buffer[i] = name[i];
	}
}

int power(int value, int power) {
	int i = 0;
	int result = 1;

	if (power == 0) {
		return 1;
	}

	for (i = 0; i < power; ++i) {
		result *= value;
	}
	return result;
}

void setNumberToBuffer(int frame, int num, int totalFrames, char *buffer, int offset) {
	int i = 0;
	int j = 0;
	int n = 0;
	int divisor = power(10, num - 1);
	int modOperand = 1;
	int modValue = 0;
	char val[2];

	printf("entering cycle in set number to buffer\n");
	for (i = 0; i < num; ++i) {
		n = frame / divisor;
		modValue = modOperand;
		if (modValue != 1) {
			for (j = 0; j < i + 1; ++j) {
				n = n % modValue;
				modValue / 10;
			}
		}
		divisor /= 10;
		modOperand *= 10;
		itoa(n,val,10);
		//suffix[i] = val[0];
		buffer[i + offset] = val[0];
	}
}

void printImageData(IplImage *img) {
	printf("image depth is: %d, number of channels: %d\n", img->depth, img->nChannels);
}

int main(int argc, char** argv)
{
	IplImage **img = NULL;
	int num = atoi( argv[2] );
	int totalFrames = atoi( argv[3] );
	char *name = argv[1];
	int i = 0;
	int h = 0, w = 0;
	int x = 0, y = 0;
	size_t length = strlen(name);
	char *buffer = (char *)malloc((length + num + 1) * sizeof(char));
	char *filename = (char *)malloc((length + num + 4 + 1) * sizeof(char));
	IplImage *result = NULL;

	if (argc != 4) {
		return -1;
	}

	buffer[length + num] = 0;
	buffer[length + num + 4] = 0;

	for (i = 0 ; i < length + num; ++i) {
		buffer[i] = 0;
	}

	printf("calling set name to buffer\n");
	setNameToBuffer(name, buffer, length);
	img = new IplImage*[totalFrames];

	printf("entering cycle 1\n");
	for (i = 0; i < totalFrames; ++i) {
		printf("before calling set number to buffer with iteration %d\n", i);
		setNumberToBuffer((i + 1), num, totalFrames, buffer, length);
		filename[0] = '\0';
		strcat(filename, buffer);
		strcat(filename, ".png");
		img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		printImageData(img[i]);
	}

	printf("after cycle, creating result image with sizes: %d x %d\n", img[0]->width * totalFrames, img[0]->height);
	result = cvCreateImage(cvSize(img[0]->width * totalFrames, img[0]->height), img[0]->depth, 4);

	printf("cv zero calling");
	cvZero(result);

	h = img[0]->height;
	w = img[0]->width;

	y = 0;
	printf("entering cycle 2");
	for (i = 0; i < totalFrames; ++i) {
		printf("iteration %d, setting image roi with y set to %d\n", i, x);
		cvSetImageROI(result, cvRect(x, y, w, h));

		printf("cv copy call\n");
		cvCopy(img[i], result);
		x += w;
	}

	
	cvResetImageROI(result);
	
	printf("saving image after the cycle\n");
	printf("end\n");
	
	cvSaveImage("result.png", result);
	return 0;
}
