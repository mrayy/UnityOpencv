
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
    PersonRecognizer(const std::string& trainingPath);
    virtual ~PersonRecognizer();
    int Recognize(const Mat &face, double &confidence) const;
    std::string GetLabel(int ID);
private:
    
    void _loadLabels(const std::string& path);
    map<int,std::string> _labels;
    Ptr<FaceRecognizer> _model;
    Size _faceSize;
};

#endif	/* PERSONRECOGNIZER_H */

