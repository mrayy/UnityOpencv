

#include "PersonRecognizer.h"
#include <fstream>
#include "Utilities.hpp"

PersonRecognizer::PersonRecognizer(const std::string& trainingPath) {
   
    //load labels ids
    _loadLabels(trainingPath+"labels.txt");
    //build recognizer model:
    _model = createEigenFaceRecognizer();
    _model->load(trainingPath+"training.xml");
    
}

PersonRecognizer::~PersonRecognizer() {}

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
int PersonRecognizer::Recognize(const Mat &face, double &confidence) const {
    Mat gray(face);
    int label=-1;
    //cvtColor(face, gray, CV_BGR2GRAY);
    resize(gray, gray, _faceSize);
    _model->predict(gray, label, confidence);
    return label;
}
std::string PersonRecognizer::GetLabel(int ID)
{
    map<int,std::string>::iterator it= _labels.find(ID);
    if(it==_labels.end())
        return "";
    return it->second;
}
