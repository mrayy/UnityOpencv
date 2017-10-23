
#include <vector>

#include "FaceDetector.h"
#include "CVVideoCapture.h"
#include <opencv/highgui.h>

const string windowName("cap window");

FaceDetector::FaceDetector(
        const string &cascadePath,
		float resizeFactor,
        double scaleFactor,
        int    minNeighbors,
        double minSizeRatio,
        double maxSizeRatio):
	_cap(0), _resizeFactor(resizeFactor),_scaleFactor(scaleFactor), _minNeighbors(minNeighbors), _minSizeRatio(minSizeRatio), _maxSizeRatio(maxSizeRatio){
	if (_cascade.load(cascadePath) == false)
	{
		printf("Failed to load cascades!\n");
	}
	namedWindow(windowName, WINDOW_AUTOSIZE);
}

FaceDetector::~FaceDetector() {}

void FaceDetector::bindImage(const Mat& img){
    //equalizeHist(img, _img);
    cv::flip(img,_img,0);
    //cv::imwrite("img.jpg",_img);
}
void FaceDetector::bindFromCamera(CVVideoCapture* cap)
{
	_cap = cap;
}
int FaceDetector::findFaces(float** pos) {
	if (_cap) {
		if (!_cap->GetFrame())
			return 0;
		cvtColor(_cap->LastFrame(), _img, CV_BGR2GRAY);
	//	imshow(windowName, _img);
	//	cv::waitKey(1);
	}
	if (_resizeFactor < 1)
		cv::resize(_img, _img, Size(_img.cols*_resizeFactor, _img.rows*_resizeFactor));
	Size minScaleSize = Size(_img.cols*_minSizeRatio, _img.rows*_minSizeRatio),// Size(70, 70), //at least 60x60 face size, to avoid noisy results
         maxScaleSize = Size(_img.cols*_maxSizeRatio, _img.rows*_maxSizeRatio);
    

	imshow(windowName, _img);
	cv::waitKey(1);
    //clear the vector:
    faces.clear();
    
    //detect faces:
    _cascade.detectMultiScale(_img, faces, _scaleFactor, _minNeighbors, 0, minScaleSize,maxScaleSize);
    
    
    facePosArr.resize(faces.size()*4);
    for(int i=0;i<faces.size();++i)
    {
        Rect& r=faces[i];
        facePosArr[i*4+0]=(r.x/ _resizeFactor);
        facePosArr[i*4+1]=(r.y / _resizeFactor);
        facePosArr[i*4+2]=(r.width / _resizeFactor);
        facePosArr[i*4+3]=(r.height / _resizeFactor);
    }
    
    if(facePosArr.size()>0)
        *pos=&facePosArr[0];
    
    return (int)(faces.size());

}
