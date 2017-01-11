
#include "Plugin.pch"
#include "defs.h"
#include "ObjectTracker.hpp"
#include "FaceDetector.h"
#include "PersonRecognizer.h"

void* FaceDetector_Create()
{
    return new FaceDetector(string(CASCADE_PATH), DET_SCALE_FACTOR, DET_MIN_NEIGHBORS, DET_MIN_SIZE_RATIO, DET_MAX_SIZE_RATIO);
}
void FaceDetector_Destroy(void* ptr)
{
    delete (FaceDetector*)ptr;
}
void FaceDetector_BindImage(void* ptr,ImageInfo* ifo){
    ((FaceDetector*)ptr)->bindImage(cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData));
}
int FaceDetector_DetectFaces(void* ptr,float**pos){
    
    return ((FaceDetector*)ptr)->findFaces(pos);

}


void* FaceRecognizer_Create(const char* trainingPath)
{
    return new PersonRecognizer(trainingPath);
}
void FaceRecognizer_Destroy(void* ptr)
{
    delete (PersonRecognizer*)ptr;
}
void FaceRecognizer_BindImage(void* ptr,ImageInfo* ifo)
{
    return ((PersonRecognizer*)ptr)->bindImage(cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData));
}
int FaceRecognizer_Recognize(void* ptr,float& confidence,float*face)
{
    return ((PersonRecognizer*)ptr)->Recognize(confidence,face);
}
const char* FaceRecognizer_GetLabel(void*ptr,int ID)
{
    
    return ((PersonRecognizer*)ptr)->GetLabel(ID).c_str();
}



void* ObjectTracker_Create()
{
    return new ObjectTracker();
}

void ObjectTracker_Destroy(void* ptr)
{
    ObjectTracker* o=(ObjectTracker*)ptr;
    delete o;
    
}

void ObjectTracker_BindImage(void* ptr,ImageInfo* ifo)
{
    
    ObjectTracker* f=(ObjectTracker*)ptr;
    if(f)
    {
        f->SetImage(ifo);
    }
}

bool ObjectTracker_TrackInImage(void* ptr,ImageInfo* ifo,float& retX,float& retY)
{
    ObjectTracker* f=(ObjectTracker*)ptr;
    if(f)
    {
        return f->Detect(ifo,retX,retY);
    }
    return false;
}
