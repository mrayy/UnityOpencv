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

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void read_training_set(const string &list_path, vector<Mat> &images,vector<int>& labels) {
    
    ifstream file((list_path+"list").c_str());
    string line;
    string path;
    int label;
    while (getline(file, line)) {
        std::vector<string> elems= split(line,':');
        if(elems.size()==0)
            continue;
        path=elems[0];
        if(elems.size()==1)
        {
            label=1;
        }else
            label=atoi(elems[1].c_str());;
        images.push_back(imread(list_path+path, CV_LOAD_IMAGE_GRAYSCALE));
        labels.push_back(label);
    }
}

bool OpenCVFaceAPI::Init(const char* trainingPath)
{
    fd=new FaceDetector(string(CASCADE_PATH), DET_SCALE_FACTOR, DET_MIN_NEIGHBORS, DET_MIN_SIZE_RATIO, DET_MAX_SIZE_RATIO);
    
    string path=string(trainingPath);
    if(path!="")
    {
        read_training_set(path, training_set,training_labels);
    
        pr=new PersonRecognizer(training_set,training_labels, LBPH_RADIUS, LBPH_NEIGHBORS, LBPH_GRID_X, LBPH_GRID_Y, LBPH_THRESHOLD);
    
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
int OpenCVFaceAPI::RecognizeFaces(float* f)
{
   
    if(pr==0)
        return -1;
    //analyze each detected face:
    Rect face=Rect(f[0],f[1],f[2],f[3]);
    Mat face_img = _internalImage(face);
    bool face_match = false;
    double confidence = 0;
    
    //try to recognize the face:
    int label=pr->recognize(face_img, confidence);
    
    
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
