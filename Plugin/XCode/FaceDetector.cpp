
#include <vector>

#include "FaceDetector.h"
#include <opencv/highgui.h>

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

void FaceDetector::bindImage(const Mat& img){
    equalizeHist(img, _img);
    cv::flip(_img,_img,0);
    //cv::imwrite("/Users/yamen.s/Documents/img.jpg",_img);
}
int FaceDetector::findFaces(float** pos) {
    
    Size minScaleSize = Size(60, 60), //at least 60x60 face size, to avoid noisy results
         maxScaleSize = Size(300,300);
    
    
    //clear the vector:
    faces.clear();
    
    //detect faces:
    _cascade.detectMultiScale(_img, faces, _scaleFactor, _minNeighbors, 0, minScaleSize,maxScaleSize);
    
    
    facePosArr.resize(faces.size()*4);
    for(int i=0;i<faces.size();++i)
    {
        Rect& r=faces[i];
        facePosArr[i*4+0]=(r.x);
        facePosArr[i*4+1]=(r.y);
        facePosArr[i*4+2]=(r.width);
        facePosArr[i*4+3]=(r.height);
    }
    
    if(facePosArr.size()>0)
        *pos=&facePosArr[0];
    
    return (int)(faces.size());

}
