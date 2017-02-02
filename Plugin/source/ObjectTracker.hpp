//
//  ObjectTracker.hpp
//  UnityOpenCV
//
//  Created by MHD Yamen Saraiji on 1/3/17.
//
//

#ifndef ObjectTracker_hpp
#define ObjectTracker_hpp

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp> // for homography
#include "ImageInfo.h"

using namespace cv;
using namespace mray;

class ObjectTracker
{
    Mat _srcImg;
    cv::Ptr<cv::FeatureDetector> _detector;
    cv::Ptr<cv::DescriptorExtractor> _extractor;
    
    std::vector<cv::KeyPoint> _objectKeypoints;
    cv::Mat _objectDescriptors;
    Mat img_matches;

public:
    ObjectTracker();
    ~ObjectTracker();
    void SetImage(video::ImageInfo* ifo);
    bool Detect(video::ImageInfo* ifo,float& retX,float& retY);
};

#endif /* ObjectTracker_hpp */
