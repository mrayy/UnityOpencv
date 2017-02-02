
#include "Plugin.h"
#include "defs.h"
#include "ObjectTracker.hpp"
#include "FaceDetector.h"
#include "PersonRecognizer.h"
#include "DarkNetDetector.h"

UNITY_INTERFACE_EXPORT void* FaceDetector_Create(const char* cascadePath, float scaler,int minNeighbors, float minScaler, float maxScaler)
{
    return new FaceDetector(string(cascadePath), scaler, minNeighbors, minScaler, maxScaler);
}
UNITY_INTERFACE_EXPORT void FaceDetector_Destroy(void* ptr)
{
    delete (FaceDetector*)ptr;
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