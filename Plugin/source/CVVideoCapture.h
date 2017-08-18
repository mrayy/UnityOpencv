
#ifndef __CVVideoCapture__
#define __CVVideoCapture__

#include "opencv2/opencv.hpp"
#include "ImageInfo.h"

using namespace mray;

class CVVideoCapture
{
	cv::VideoCapture *_captureDevice;
	int _index;
	cv::Mat _frame;
	int _width;
	int _height;
public:
	CVVideoCapture();
	virtual ~CVVideoCapture();

	bool Open(int device);

	void SetSize(int width, int height);
	void Close();
	bool IsOpen();

	bool GetFrame();
	cv::Mat LastFrame() { return _frame; }

	bool ToImage(video::ImageInfo* ifo, int x, int y, int w, int h);
};

#endif

