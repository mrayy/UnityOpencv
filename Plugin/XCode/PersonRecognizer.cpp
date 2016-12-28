/* 
 * File:   PersonRecognizer.cpp
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#include "PersonRecognizer.h"

PersonRecognizer::PersonRecognizer(const vector<Mat> &imgs,const vector<int>& labels, int radius, int neighbors,
        int grid_x, int grid_y, double threshold) {
    //all images are faces of the same person, so initialize the same label for all.
    _faceSize = Size(imgs[0].size().width, imgs[0].size().height);
    
    //build recognizer model:
    _model = createLBPHFaceRecognizer(radius, neighbors, grid_x, grid_y, threshold);
    _model->train(imgs, labels);
}

PersonRecognizer::~PersonRecognizer() {}

int PersonRecognizer::recognize(const Mat &face, double &confidence) const {
    Mat gray(face);
    int label=-1;
    //cvtColor(face, gray, CV_BGR2GRAY);
    resize(gray, gray, _faceSize);
    _model->predict(gray, label, confidence);
    return label;
}
