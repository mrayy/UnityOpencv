/* 
 * File:   PersonRecognizer.h
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#ifndef PERSONRECOGNIZER_H
#define	PERSONRECOGNIZER_H

#define PERSON_LABEL 10 //some arbitrary label

#include <string>
#include "opencv/cv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;

class PersonRecognizer {
public:
    PersonRecognizer(const vector<Mat> &imgs, const vector<int>& labels, int radius, int neighbors,
            int grid_x, int grid_y, double threshold);
    virtual ~PersonRecognizer();
    int recognize(const Mat &face, double &confidence) const;
private:
    Ptr<FaceRecognizer> _model;
    Size _faceSize;
};

#endif	/* PERSONRECOGNIZER_H */

