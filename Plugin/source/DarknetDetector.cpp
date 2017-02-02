
 
#include "DarkNetDetector.h"
#include "DarknetAPI.h"

DarknetDetector::DarknetDetector(
	const string &datacfg,
	const string &cfgfile,
	const string &weightfile,
	float threshold, int gpuIndex)
{
	_imgArray = 0;
	_w = 0;
	_h = 0;
	_instance = DN_CreateDetectorInstance((char*)datacfg.c_str(), (char*)cfgfile.c_str(), (char*)weightfile.c_str(), threshold,gpuIndex);
}
DarknetDetector::~DarknetDetector()
{
	DN_DestroyDetectorInstance(_instance);

	delete[] _imgArray;
}

void DarknetDetector::bindImage(video::ImageInfo* ifo)
{
	int len = ifo->Size.x*ifo->Size.y;
	if (_w*_h!=len)
	{
		delete[] _imgArray;
		_imgArray = new float[len*3];
	}

	_w = ifo->Size.x;
	_h = ifo->Size.y;

	float* ptrDstA = _imgArray;
	float* ptrDstB = _imgArray+len;
	float* ptrDstC = _imgArray+2*len;
	int pitch = _w * 3;
	unsigned char* ptrSrc = ifo->imageData + pitch*(_h - 1);
	const float inv = 1.0f / 255.0f;

	int x = 0;
	for (int i = 0; i < len; ++i)
	{
		*ptrDstA = (ptrSrc[0])*inv;
		*ptrDstB = (ptrSrc[1])*inv;
		*ptrDstC = (ptrSrc[2])*inv;

		++ptrDstA;
		++ptrDstB;
		++ptrDstC;

		++x;
		ptrSrc+=3;

		if (x >= _w)
		{
			x = 0;
			ptrSrc -= 2* pitch;
		}
	}

}
int DarknetDetector::Predict(float** regions, float**prob, int**classes)
{
	int count=DN_PredictDetectorLabels(_instance, _imgArray, _w, _h);

	_regionsArr.resize(count*4);
	_probabilities.resize(count);
	_classes.resize(count);
	if (count > 0)
	{
		DN_CopyDetectorBoxes(_instance, &_regionsArr[0], &_classes[0], &_probabilities[0]);
		*regions = &_regionsArr[0];
		*classes = &_classes[0];
		*prob = &_probabilities[0];
	}
		

	return count;
}
char* DarknetDetector::GetClassName( int ID)
{
	return DN_GetClassName(_instance, ID);
}