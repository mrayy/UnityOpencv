//
//  OpenCVFaceAPI.h
//  UnityOpenCV
//
//  Created by MHD Yamen Saraiji on 12/27/16.
//
//

#ifndef OpenCVFaceAPI_h
#define OpenCVFaceAPI_h


#include <vector>
#include <fstream>

#include "FaceDetector.h"
#include "PersonRecognizer.h"
#include "defs.h"
#include "ImageInfo.h"

using namespace cv;
using namespace std;
using namespace mray;

class OpenCVFaceAPI
{
protected:
    
    FaceDetector* fd;
    vector<Mat>  training_set;
    vector<int>  training_labels;
    vector<Rect> faces;
    
    PersonRecognizer *pr;
    
    vector<float> facePosArr;
    
    Mat _internalImage;
    
public:
    bool Init(const char* trainingPath);
    void Shutdown();
    
    void SetImage(video::ImageInfo* ifo);
    
    int DetectFaces(float**pos);
    int RecognizeFace(float* face);
    std::string GetFaceLabel(int ID);
};
#endif /* OpenCVFaceAPI_h */
