
#include "Plugin.pch"
#include "OpenCVFaceAPI.h"
#include "ObjectTracker.hpp"

void* CreateInstance(const char* trainingPath)
{
    OpenCVFaceAPI* f= new OpenCVFaceAPI();
    f->Init(trainingPath);
    return f;
}
void DestroyInstance(void* ptr)
{
    OpenCVFaceAPI* f=(OpenCVFaceAPI*)ptr;
    if(f)
    {
        delete f;
        f=0;
    }
}
void BindImage(void* ptr,ImageInfo* ifo)
{
    OpenCVFaceAPI* f=(OpenCVFaceAPI*)ptr;
    if(f)
    {
        f->SetImage(ifo);
    }
}
int DetectFaces(void* ptr,float**pos)
{
    
    OpenCVFaceAPI* f=(OpenCVFaceAPI*)ptr;
    if(f)
    {
        return f->DetectFaces(pos);
    }
    return 0;
}
int RecognizeFace(void* ptr,float*face)
{
    
    OpenCVFaceAPI* f=(OpenCVFaceAPI*)ptr;
    if(f)
    {
        return f->RecognizeFace(face);
    }
    return 0;
}

const char* GetFaceLabel(void*ptr,int ID)
{
    
    OpenCVFaceAPI* f=(OpenCVFaceAPI*)ptr;
    if(f)
    {
        static string tmp;
        tmp=f->GetFaceLabel(ID);
        return tmp.c_str();
    }
    return "";
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