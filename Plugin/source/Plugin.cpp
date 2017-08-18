
#include "Plugin.h"
#include "defs.h"
#include "ObjectTracker.hpp"
#include "FaceDetector.h"
#include "PersonRecognizer.h"
#ifdef USE_DN
#include "DarkNetDetector.h"
#endif
#include "CVVideoCapture.h"

UNITY_INTERFACE_EXPORT void* FaceDetector_Create(const char* cascadePath, float resizeFactor, float scaler,int minNeighbors, float minScaler, float maxScaler)
{
    return new FaceDetector(string(cascadePath), resizeFactor, scaler, minNeighbors, minScaler, maxScaler);
}
UNITY_INTERFACE_EXPORT void FaceDetector_Destroy(void* ptr)
{
    delete (FaceDetector*)ptr;
}
UNITY_INTERFACE_EXPORT void FaceDetector_BindCamera(void* ptr, void* cam)
{
	((FaceDetector*)ptr)->bindFromCamera((CVVideoCapture*)cam);
}
UNITY_INTERFACE_EXPORT void FaceDetector_BindImage(void* ptr,ImageInfo* ifo){
    ((FaceDetector*)ptr)->bindImage(cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData));
}
UNITY_INTERFACE_EXPORT int FaceDetector_DetectFaces(void* ptr,float**pos){
    
    return ((FaceDetector*)ptr)->findFaces(pos);

}


UNITY_INTERFACE_EXPORT void* FaceRecognizer_Create(const char* trainingPath)
{
    return new PersonRecognizer(trainingPath);
}
UNITY_INTERFACE_EXPORT void FaceRecognizer_Destroy(void* ptr)
{
    delete (PersonRecognizer*)ptr;
}
UNITY_INTERFACE_EXPORT void FaceRecognizer_BindImage(void* ptr,ImageInfo* ifo)
{
    return ((PersonRecognizer*)ptr)->bindImage(cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData));
}
UNITY_INTERFACE_EXPORT int FaceRecognizer_Recognize(void* ptr,float& confidence,float*face)
{
    return ((PersonRecognizer*)ptr)->Recognize(confidence,face);
}
UNITY_INTERFACE_EXPORT const char* FaceRecognizer_GetLabel(void*ptr,int ID)
{
    
    return ((PersonRecognizer*)ptr)->GetLabel(ID);
}



UNITY_INTERFACE_EXPORT void* ObjectTracker_Create()
{
    return new ObjectTracker();
}

UNITY_INTERFACE_EXPORT void ObjectTracker_Destroy(void* ptr)
{
    ObjectTracker* o=(ObjectTracker*)ptr;
    delete o;
    
}

UNITY_INTERFACE_EXPORT void ObjectTracker_BindImage(void* ptr,ImageInfo* ifo)
{
    
    ObjectTracker* f=(ObjectTracker*)ptr;
    if(f)
    {
        f->SetImage(ifo);
    }
}

UNITY_INTERFACE_EXPORT bool ObjectTracker_TrackInImage(void* ptr,ImageInfo* ifo,float& retX,float& retY)
{
    ObjectTracker* f=(ObjectTracker*)ptr;
    if(f)
    {
        return f->Detect(ifo,retX,retY);
    }
    return false;
}

#ifdef USE_DN

UNITY_INTERFACE_EXPORT void* DN_Create(const char* datacfg, const char* cfgfile, const char* weightfile, float threshold, int gpuIndex) {
	
	DarknetDetector* inst = new DarknetDetector(datacfg,cfgfile,weightfile,threshold,gpuIndex);

	return inst;
}
UNITY_INTERFACE_EXPORT void DN_Destroy(void* ptr)
{

	DarknetDetector* inst = (DarknetDetector*)ptr;
	delete inst;
}
UNITY_INTERFACE_EXPORT void DN_BindImage(void* ptr, ImageInfo* ifo)
{
	DarknetDetector* inst = (DarknetDetector*)ptr;
	inst->bindImage(ifo);
}
UNITY_INTERFACE_EXPORT int DN_Predict(void* ptr, float**regions, float**prob, int**classes) {

	DarknetDetector* inst = (DarknetDetector*)ptr;
	return inst->Predict(regions,prob,classes);
}
UNITY_INTERFACE_EXPORT char* DN_GetClass(void* ptr, int ID)
{
	DarknetDetector* inst = (DarknetDetector*)ptr;
	return inst->GetClassName( ID);

}

#endif

UNITY_INTERFACE_EXPORT void* CVVideoCap_Create()
{
	return new CVVideoCapture();
}
UNITY_INTERFACE_EXPORT bool CVVideoCap_Open(void*cap, int index)
{
	return ((CVVideoCapture*)cap)->Open(index);
}
UNITY_INTERFACE_EXPORT bool CVVideoCap_IsOpen(void*cap)
{
	return ((CVVideoCapture*)cap)->IsOpen();
}
UNITY_INTERFACE_EXPORT void CVVideoCap_Close(void*cap)
{
	((CVVideoCapture*)cap)->Close();
}
UNITY_INTERFACE_EXPORT void CVVideoCap_Destroy(void*cap)
{
	delete ((CVVideoCapture*)cap);

}
UNITY_INTERFACE_EXPORT void CVVideoCap_SetSize(void*cap, int width, int height)
{

	((CVVideoCapture*)cap)->SetSize(width,height);
}
UNITY_INTERFACE_EXPORT void CVVideoCap_Capture(void*cap)
{
	((CVVideoCapture*)cap)->GetFrame();
}
UNITY_INTERFACE_EXPORT bool CVVideoCap_ToImage(void* cap, ImageInfo* ifo, int x, int y, int w, int h)
{
	return ((CVVideoCapture*)cap)->ToImage(ifo,x,y,w,h);
}



/////////////////

extern "C" UNITY_INTERFACE_EXPORT video::ImageInfo* mray_createImageData(int width, int height, video::EPixelFormat format)
{
	video::ImageInfo* ifo = new video::ImageInfo();
	ifo->createData(Vector2d(width, height), format);
	return ifo;
}
extern "C" UNITY_INTERFACE_EXPORT void mray_resizeImageData(video::ImageInfo* ifo, int width, int height, video::EPixelFormat format)
{
	ifo->createData(Vector2d(width, height), format);
}
extern "C" UNITY_INTERFACE_EXPORT void mray_getImageDataInfo(video::ImageInfo* ifo, int& width, int& height, video::EPixelFormat&  format)
{
	width = ifo->Size.x;
	height = ifo->Size.y;
	format = ifo->format;
}

extern "C" UNITY_INTERFACE_EXPORT void mray_cloneImageData(video::ImageInfo* ifo, video::ImageInfo* dst)
{
	dst->copyFrom(ifo);
}
extern "C" UNITY_INTERFACE_EXPORT void mray_copyCroppedImageData(video::ImageInfo* ifo, video::ImageInfo* dst, int x, int y, int width, int height, bool clamp)
{
	dst->copyCroppedFrom(ifo, Vector2d(x, y), Vector2d(width, height), clamp);
}
extern "C" UNITY_INTERFACE_EXPORT void mray_deleteImageData(video::ImageInfo* ifo)
{
	if (ifo != NULL)
		delete ifo;
	ifo = NULL;
}

extern "C" UNITY_INTERFACE_EXPORT void mray_BlitImageDataInfo(video::ImageInfo* ifo, void* _TextureNativePtr)
{

	if (ifo == NULL || !_TextureNativePtr)
		return;

	if (ifo)
	{
		BlitImage(ifo, _TextureNativePtr, ifo->Size.x, ifo->Size.y);
	}

}

extern "C" UNITY_INTERFACE_EXPORT void* mray_getImageDataPtr(video::ImageInfo* ifo)
{

	if (ifo == NULL)
		return 0;
	return ifo->imageData;
}

extern "C" UNITY_INTERFACE_EXPORT void mray_FlipImageData(video::ImageInfo* ifo, bool horizontal, bool vertical)
{
	if (ifo == NULL)
		return;
	ifo->FlipImage(horizontal, vertical);

}


struct ImageBlitData
{
	video::ImageInfo* ifo;
	void* _TextureNativePtr;
};

std::vector<ImageBlitData> _data;

static void  __stdcall mray_gst_customPlayerBlitImageNativeEvent(int eventID)
{
	if (_data.size() == 0)
		return;
	for (int i = 0; i < _data.size(); ++i)
	{
		ImageBlitData& r = _data[i];
		mray_BlitImageDataInfo(r.ifo, r._TextureNativePtr);
	}
	_data.clear();
}
extern "C" UNITY_INTERFACE_EXPORT UnityRenderNative mray_BlitImageNativeGLCall(video::ImageInfo* ifo, void* _TextureNativePtr)
{
	ImageBlitData r;
	r.ifo = ifo;
	r._TextureNativePtr = _TextureNativePtr;
	_data.push_back(r);
	return mray_gst_customPlayerBlitImageNativeEvent;
}
