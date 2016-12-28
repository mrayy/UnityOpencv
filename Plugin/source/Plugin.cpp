/*
	This is a simple plugin, a bunch of functions that do simple things.
*/

#include "Plugin.pch"
#include "OpenCVFaceAPI.h"

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
    }
    return 0;
}
