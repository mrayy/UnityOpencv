//
//  OpenCVFaceAPI.cpp
//  UnityOpenCV
//
//  Created by MHD Yamen Saraiji on 12/27/16.
//
//

#include <stdio.h>
#include "OpenCVFaceAPI.h"

#include <opencv/cv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <cstdlib>
bool OpenCVFaceAPI::Init(const char* trainingPath)
{
    fd=new FaceDetector(string(CASCADE_PATH), DET_SCALE_FACTOR, DET_MIN_NEIGHBORS, DET_MIN_SIZE_RATIO, DET_MAX_SIZE_RATIO);
    
    if(string(trainingPath)!="")
    {
        pr=new PersonRecognizer(trainingPath);
    
    }else
        pr=0;
    return true;
}
void OpenCVFaceAPI::Shutdown()
{
    delete fd;
    delete pr;
    fd=0;
    pr=0;
}

void OpenCVFaceAPI::SetImage(video::ImageInfo* ifo)
{
    _internalImage=cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData);
    
    
}
int OpenCVFaceAPI::RecognizeFace(float* f)
{
   
    if(pr==0)
        return -1;
    //analyze each detected face:
    Rect face=Rect(f[0],f[1],f[2],f[3]);
    Mat face_img = _internalImage(face);
    bool face_match = false;
    double confidence = 0;
    
    //try to recognize the face:
    int label=pr->Recognize(face_img, confidence);
    
    
    return label;
}

int OpenCVFaceAPI::DetectFaces(float**pos)
{
    
    
    //imwrite("/Users/yamen.s/Documents/test.png",im);
    fd->findFacesInImage(_internalImage, faces);
    facePosArr.resize(faces.size()*4);
    for(int i=0;i<faces.size();++i)
    {
        Rect& r=faces[i];
        facePosArr[i*4+0]=(r.x);
        facePosArr[i*4+1]=(r.y);
        facePosArr[i*4+2]=(r.width);
        facePosArr[i*4+3]=(r.height);
    }
    
    if(facePosArr.size()>0)
        *pos=&facePosArr[0];
    
    return (int)(faces.size());
    
}

std::string OpenCVFaceAPI::GetFaceLabel(int ID)
{
    if(pr==0)
        return "";
    return pr->GetLabel(ID);
}
