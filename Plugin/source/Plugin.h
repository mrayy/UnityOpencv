
#include "ImageInfo.h"

#if defined(__CYGWIN32__)
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(__QNX__)
#define UNITY_INTERFACE_API
#define UNITY_INTERFACE_EXPORT
#else
#define UNITY_INTERFACE_API
#define UNITY_INTERFACE_EXPORT
#endif



using namespace mray;
using namespace video;
extern "C"  {
    
	UNITY_INTERFACE_EXPORT void* FaceDetector_Create(const char* cascadePath, float resizeFactor,float scaler, int minNeighbors,float minScaler,float maxScaler);
	UNITY_INTERFACE_EXPORT void FaceDetector_BindCamera(void* ptr, void* cam);
	UNITY_INTERFACE_EXPORT void FaceDetector_Destroy(void* ptr);
	UNITY_INTERFACE_EXPORT void FaceDetector_BindImage(void* ptr,ImageInfo* ifo);
	UNITY_INTERFACE_EXPORT int FaceDetector_DetectFaces(void* ptr,float**pos);
    
    
	UNITY_INTERFACE_EXPORT void* FaceRecognizer_Create(const char* trainingPath);
	UNITY_INTERFACE_EXPORT void FaceRecognizer_Destroy(void* ptr);
	UNITY_INTERFACE_EXPORT void FaceRecognizer_BindImage(void* ptr,ImageInfo* ifo);
	UNITY_INTERFACE_EXPORT int FaceRecognizer_Recognize(void* ptr,float& confidence,float*face);
	UNITY_INTERFACE_EXPORT const char* FaceRecognizer_GetLabel(void*ptr,int ID);
    
	UNITY_INTERFACE_EXPORT void* ObjectTracker_Create();
	UNITY_INTERFACE_EXPORT void ObjectTracker_Destroy(void* ptr);
	UNITY_INTERFACE_EXPORT void ObjectTracker_BindImage(void* ptr, ImageInfo* ifo);
	UNITY_INTERFACE_EXPORT bool ObjectTracker_TrackInImage(void* ptr,ImageInfo* ifo,float& retX,float& retY);
#ifdef USE_DN
	UNITY_INTERFACE_EXPORT void* DN_Create(const char* datacfg, const char* cfgfile, const char* weightfile,float threshold,int gpuIndex);
	UNITY_INTERFACE_EXPORT void DN_Destroy(void* ptr);
	UNITY_INTERFACE_EXPORT void DN_BindImage(void* ptr, ImageInfo* ifo);
	UNITY_INTERFACE_EXPORT int DN_Predict(void* ptr, float**regions, float**prob, int**classes);
	UNITY_INTERFACE_EXPORT char* DN_GetClass(void* ptr, int ID);
#endif

	UNITY_INTERFACE_EXPORT void* CVVideoCap_Create();
	UNITY_INTERFACE_EXPORT bool CVVideoCap_Open(void*cap, int index);
	UNITY_INTERFACE_EXPORT bool CVVideoCap_IsOpen(void*cap);
	UNITY_INTERFACE_EXPORT void CVVideoCap_Close(void*cap);
	UNITY_INTERFACE_EXPORT void CVVideoCap_Destroy(void*cap);
	UNITY_INTERFACE_EXPORT void CVVideoCap_SetSize(void*cap,int width,int height);
	UNITY_INTERFACE_EXPORT void CVVideoCap_Capture(void*cap);
	UNITY_INTERFACE_EXPORT bool CVVideoCap_ToImage(void* cap, ImageInfo* ifo, int x, int y, int w, int h);

}
