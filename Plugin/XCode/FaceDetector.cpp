/* 
 * File:   FaceDetector.cpp
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#include <vector>

#include "FaceDetector.h"

FaceDetector::FaceDetector(
        const string &cascadePath,
        double scaleFactor,
        int    minNeighbors,
        double minSizeRatio,
        double maxSizeRatio):
    _scaleFactor(scaleFactor), _minNeighbors(minNeighbors), _minSizeRatio(minSizeRatio), _maxSizeRatio(maxSizeRatio){
    _cascade.load(cascadePath);
}

FaceDetector::~FaceDetector() {}

void FaceDetector::findFacesInImage(const Mat &img, vector<Rect> &res) {
    Mat tmp;
    int width  = img.size().width,
        height = img.size().height;
    Size minScaleSize = Size(40, 40),
         maxScaleSize = Size(300,300);
    
    //convert the image to grayscale and normalize histogram:
    tmp=img;
    equalizeHist(tmp, tmp);
    
    //clear the vector:
    res.clear();
    
    //detect faces:
    _cascade.detectMultiScale(tmp, res, 1.05, 10, 0, minScaleSize,maxScaleSize);
}
