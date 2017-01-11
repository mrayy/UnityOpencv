

#include "PersonRecognizer.h"
#include <fstream>
#include "Utilities.hpp"
#include <opencv/highgui.h>

PersonRecognizer::PersonRecognizer(const std::string& trainingPath) {
   
    //load labels ids
    _loadLabels(trainingPath+"labels.txt");
    //build recognizer model:
    //_model = createEigenFaceRecognizer();
    _model=createLBPHFaceRecognizer();
    _model->load(trainingPath+"training.xml");
    
}

PersonRecognizer::~PersonRecognizer() {}

void PersonRecognizer::bindImage(const Mat& img){
    _img=img;
    // equalizeHist(_img, _img);
    cv::flip(_img,_img,0);
    //cv::imwrite("/Users/yamen.s/Documents/img.jpg",_img);
    
}

void PersonRecognizer::_loadLabels(const std::string& path)
{
    ifstream stream;
    stream.open(path);
    if(!stream.is_open())
        return;
    
    std::string line;
    std::getline(stream,line);
    
    std::vector<std::string> parts=split(line,',');
    int faceW=atoi(parts[0].c_str());
    int faceH=atoi(parts[1].c_str());
    //all images are faces of the same person, so initialize the same label for all.
    _faceSize = Size(faceW,faceH);
    
    while(!stream.eof())
    {
        std::getline(stream,line);
        
        parts=split(line,':');
        if(parts.size()<2)
            continue;
        _labels[atoi(parts[0].c_str())]=trim(parts[1]);
    }
    stream.close();
}
int PersonRecognizer::Recognize(float &confidence,float* f) const {
    int label=-1;
    //cvtColor(face, gray, CV_BGR2GRAY);
    
    Rect face=Rect(f[0],f[1],f[2],f[3]);
    Mat face_img = _img(face);
    resize(face_img, face_img, _faceSize);
    //try to recognize the face:
    double conf;
    _model->predict(face_img, label, conf);
    confidence=conf;
    return label;
}
std::string PersonRecognizer::GetLabel(int ID)
{
    map<int,std::string>::iterator it= _labels.find(ID);
    if(it==_labels.end())
        return "";
    return it->second;
}
