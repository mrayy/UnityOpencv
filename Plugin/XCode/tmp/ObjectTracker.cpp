//
//  ObjectTracker.cpp
//  UnityOpenCV
//
//  Created by MHD Yamen Saraiji on 1/3/17.
//
//

#include "ObjectTracker.hpp"

#include <opencv2/nonfree/gpu.hpp>
#include <opencv2/nonfree/features2d.hpp>

//based on:
// https://github.com/introlab/find-object/blob/master/example/main.cpp
// http://docs.opencv.org/3.0-beta/doc/py_tutorials/py_feature2d/py_feature_homography/py_feature_homography.html

ObjectTracker::ObjectTracker()
{
    _detector=cv::Ptr<cv::FeatureDetector>(new cv::SIFT());
    _extractor=cv::Ptr<cv::DescriptorExtractor>(new cv::SIFT());

}
ObjectTracker::~ObjectTracker()
{
    
}

void ObjectTracker::SetImage(video::ImageInfo* ifo)
{
    _srcImg=cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData);
    _detector->detect(_srcImg, _objectKeypoints);
    _extractor->compute(_srcImg, _objectKeypoints, _objectDescriptors);
   // if(_objectDescriptors.type()!=CV_8U)
   //      _objectDescriptors.convertTo(_objectDescriptors, CV_8U);
    
}
bool ObjectTracker::Detect(video::ImageInfo* ifo,float& retX,float& retY)
{
    cv::Mat scene=cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData);
    std::vector<cv::KeyPoint> sceneKeypoints;
    cv::Mat sceneDescriptors;
    cv::Mat results;
    cv::Mat dists;
    std::vector<std::vector<cv::DMatch> > matches;
    int k=2;

    _detector->detect(scene, sceneKeypoints);
    if(sceneKeypoints.size()==0)
        return false;
    _extractor->compute(scene, sceneKeypoints, sceneDescriptors);
    
   // if(sceneDescriptors.type()!=CV_8U)
   //     sceneDescriptors.convertTo(sceneDescriptors, CV_8U);
        
   // cv::flann::Index flannIndex(sceneDescriptors,cv::flann::LshIndexParams(12,20,2),cvflann::FLANN_DIST_HAMMING);
    //flannIndex.knnSearch(_objectDescriptors, results, dists, k,cv::flann::SearchParams());
    
    if(sceneDescriptors.type()==CV_32F)
    {
        cv::flann::Index flannIndex(sceneDescriptors, cv::flann::KDTreeIndexParams(), cvflann::FLANN_DIST_EUCLIDEAN);
	
        flannIndex.knnSearch(_objectDescriptors, results, dists, k, cv::flann::SearchParams() );
    }else if(sceneDescriptors.type()==CV_8U){
        
        // Create Flann LSH index
        cv::flann::Index flannIndex(sceneDescriptors, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);
        
        // search (nearest neighbor)
        flannIndex.knnSearch(sceneDescriptors, results, dists, k, cv::flann::SearchParams() );
    }
    if(dists.type() == CV_32S)
    {
        cv::Mat temp;
        dists.convertTo(temp, CV_32F);
        dists = temp;
    }
    // Find correspondences by NNDR (Nearest Neighbor Distance Ratio)
    float nndrRatio = 0.8f;
    std::vector<cv::Point2f> mpts_1, mpts_2; // Used for homography
    std::vector<int> indexes_1, indexes_2; // Used for homography
    std::vector<uchar> outlier_mask;  // Used for homography
    // Check if this descriptor matches with those of the objects
  
        for(int i=0; i<_objectDescriptors.rows; ++i)
        {
            // Apply NNDR
            //printf("q=%d dist1=%f dist2=%f\n", i, dists.at<float>(i,0), dists.at<float>(i,1));
            if(results.at<int>(i,0) >= 0 && results.at<int>(i,1) >= 0 &&
               dists.at<float>(i,0) <= nndrRatio * dists.at<float>(i,1))
            {
                mpts_1.push_back(_objectKeypoints.at(i).pt);
                indexes_1.push_back(i);
                
                mpts_2.push_back(sceneKeypoints.at(results.at<int>(i,0)).pt);
                indexes_2.push_back(results.at<int>(i,0));
            }
        }
    // FIND HOMOGRAPHY
    unsigned int minInliers = 8;
    if(mpts_1.size() >= minInliers)
    {
        cv::Mat H = findHomography(mpts_1,
                                   mpts_2,
                                   cv::RANSAC,
                                   1.0,
                                   outlier_mask);
       
        std::vector<Point2f> points(1);
        std::vector<Point2f> proj_corners;
        points[0] = cvPoint(0,0);//_srcImg.cols/2,_srcImg.rows/2);

        cv::perspectiveTransform( points, proj_corners,H);
        
        retX=proj_corners[0].x;
        retY=proj_corners[0].y;
        /*
        int inliers=0, outliers=0;
        for(unsigned int k=0; k<mpts_1.size();++k)
        {
            if(outlier_mask.at(k))
            {
                ++inliers;
            }
            else
            {
                ++outliers;
            }
        }*/
        /*QTransform hTransform(
                              H.at<double>(0,0), H.at<double>(1,0), H.at<double>(2,0),
                              H.at<double>(0,1), H.at<double>(1,1), H.at<double>(2,1),
                              H.at<double>(0,2), H.at<double>(1,2), H.at<double>(2,2));*/
        
    }
    else
    {
       // printf("Not enough matches (%d) for homography...\n", (int)mpts_1.size());
        return false;
    }
    
    return true;
}
