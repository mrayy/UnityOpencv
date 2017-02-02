#pragma once


#ifndef DARKNETDETECTOR_H
#define	DARKNETDETECTOR_H

#include <string>
#include "ImageInfo.h"
#include "opencv/cv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace mray;

class DarknetDetector {
public:
	DarknetDetector(
		const string &datacfg,
		const string &cfgfile, 
		const string &weightfil,
		float threshold,
		int gpuIndex);
	virtual ~DarknetDetector();

	void bindImage(video::ImageInfo* ifo);
	int Predict(float** regions,float**prob,int**classes);
	char* GetClassName( int ID);
private:

	void* _instance;

	float* _imgArray;
	int _w;
	int _h;

	vector<Rect> _regions;
	vector<float> _regionsArr;
	vector<float> _probabilities;
	vector<int> _classes;
};

#endif	/* DARKNETDETECTOR_H */



