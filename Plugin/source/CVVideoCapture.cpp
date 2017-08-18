

#include "stdafx.h"
#include "CVVideoCapture.h"



CVVideoCapture::CVVideoCapture()
{
	_index = -1;
	_captureDevice = 0;
}
CVVideoCapture::~CVVideoCapture()
{
	Close();
}

bool CVVideoCapture::Open(int device)
{
	if (_index == device && _captureDevice)
		return true;
	if (_captureDevice)
		Close();
	_index = device;
	_captureDevice = new cv::VideoCapture(_index);
	if (!_captureDevice->isOpened())
		return false;
	_width = _captureDevice->get(CV_CAP_PROP_FRAME_WIDTH);
	_height = _captureDevice->get(CV_CAP_PROP_FRAME_HEIGHT);
	return true;
}
void CVVideoCapture::SetSize(int width, int height)
{
	if (!IsOpen())return;
	_captureDevice->set(CV_CAP_PROP_FRAME_WIDTH, width);
	_captureDevice->set(CV_CAP_PROP_FRAME_HEIGHT, height);

	_width = _captureDevice->get(CV_CAP_PROP_FRAME_WIDTH);
	_height = _captureDevice->get(CV_CAP_PROP_FRAME_HEIGHT);
}
void CVVideoCapture::Close()
{
	if (!_captureDevice)
		return;
	_captureDevice->release();
//	delete _captureDevice;
	_captureDevice = 0;
	_index = 0;
}
bool CVVideoCapture::IsOpen()
{
	return _captureDevice && _captureDevice->isOpened();
}

bool CVVideoCapture::GetFrame()
{
	if (!IsOpen())
		return false;
	(*_captureDevice) >> _frame;
	return _frame.rows>0 && _frame.cols>0 && _frame.data!=0;
}

bool CVVideoCapture::ToImage(video::ImageInfo* ifo, int x, int y, int w, int h)
{
	if (!IsOpen())return false;
	if (x > 0 || y > 0 || w < _width || h < _height) {
		cv::Mat m(_frame, cv::Rect(x, y, w, h));
		cv::Mat croppedImage;
		m.copyTo(croppedImage);
		flip(croppedImage, croppedImage,0);
		ifo->setData(croppedImage.data, Vector2d(w, h), video::EPixel_B8G8R8);
	}
	else ifo->setData(_frame.data, Vector2d(_width,_height), video::EPixel_B8G8R8);
	return true;
}