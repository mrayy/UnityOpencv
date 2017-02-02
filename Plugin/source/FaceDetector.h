
#ifndef FACEDETECTOR_H
#define	FACEDETECTOR_H

#include <string>
#include "opencv/cv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class FaceTemplate
{
public:
	int ID;
	Mat _template;
	float timeout;
};

class FaceDetector {
public:
    FaceDetector(
            const string &cascadePath,
            double scaleFactor,
            int    minNeighbors,
            double minSizeRatio,
            double maxSizeRatio);
    virtual ~FaceDetector();
    
    void bindImage(const Mat& img);
    int findFaces(float** faces);//vector<Rect> &res);
private:
    Mat _img;
    CascadeClassifier _cascade;
    double _scaleFactor;
    int    _minNeighbors;
    double _minSizeRatio;
    double _maxSizeRatio;
    
    vector<Rect> faces;
    vector<float> facePosArr;
};

#endif	/* FACEDETECTOR_H */

