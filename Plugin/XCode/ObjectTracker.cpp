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
#include <opencv2/imgproc/imgproc.hpp>

#include <unistd.h>

bool _debug=false;

ObjectTracker::ObjectTracker()
{
    int minHessian = 400;
    _detector=cv::Ptr<cv::FeatureDetector>(new cv::SurfFeatureDetector(minHessian));
    _extractor=cv::Ptr<cv::DescriptorExtractor>(new cv::SurfDescriptorExtractor());
    
   // destroyWindow("matches");
}
ObjectTracker::~ObjectTracker()
{
}

void ObjectTracker::SetImage(video::ImageInfo* ifo)
{
    _srcImg=cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData).clone();
    cv::equalizeHist(_srcImg,_srcImg);
    _detector->detect(_srcImg, _objectKeypoints);
    _extractor->compute(_srcImg, _objectKeypoints, _objectDescriptors);
   // if(_objectDescriptors.type()!=CV_8U)
   //      _objectDescriptors.convertTo(_objectDescriptors, CV_8U);
    
}
bool ObjectTracker::Detect(video::ImageInfo* ifo,float& retX,float& retY)
{
    cv::Mat scene=cv::Mat(ifo->Size.x,ifo->Size.y,CV_8U,(void*)ifo->imageData);
    cv::equalizeHist(scene,scene );

    std::vector<cv::KeyPoint> sceneKeypoints;
    cv::Mat sceneDescriptors;
    std::vector<cv::DMatch>  matches;
    int k=2;
    
    std::vector<DMatch > good_matches;
    
    _detector->detect(scene, sceneKeypoints);
    if(sceneKeypoints.size()==0)
        return false;
    _extractor->compute(scene, sceneKeypoints, sceneDescriptors);
    
    cv::FlannBasedMatcher matcher;
    matcher.match(_objectDescriptors, sceneDescriptors, matches);
    
    double max_dist = 0; double min_dist = 100;
    
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < _objectDescriptors.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    for(int i=0;i<_objectDescriptors.rows;++i)
    {
        if(matches[i].distance<3*min_dist)
        {
            good_matches.push_back(matches[i]);
        }
    }
    
    if(_debug){
        cv::drawMatches( _srcImg, _objectKeypoints, scene, sceneKeypoints,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    }
    std::vector<cv::Point2f> mpts_1, mpts_2; // Used for homography
    for(int i=0;i<good_matches.size();++i)
    {
        mpts_1.push_back(_objectKeypoints[good_matches[i].queryIdx].pt);
        mpts_2.push_back(sceneKeypoints[good_matches[i].trainIdx].pt);
    }
    unsigned int minInliers = 8;
    // FIND HOMOGRAPHY
    if(mpts_1.size() >= minInliers)
    {
        cv::Mat H = findHomography(mpts_1,
                                   mpts_2,
                                   cv::RANSAC);
       
        std::vector<Point2f> points(1);
        std::vector<Point2f> proj_corners;
        points[0] = cvPoint(_srcImg.cols/2,_srcImg.rows/2);

        cv::perspectiveTransform( points, proj_corners,H);
        
        retX=proj_corners[0].x;
        retY=proj_corners[0].y;
        
        if(_debug)
        {
        
            //-- Get the corners from the image_1 ( the object to be "detected" )
            std::vector<Point2f> obj_corners(4);
            obj_corners[0] = cvPoint(0,0);
            obj_corners[1] = cvPoint( _srcImg.cols, 0 );
            obj_corners[2] = cvPoint( _srcImg.cols, _srcImg.rows );
            obj_corners[3] = cvPoint( 0, _srcImg.rows );
            std::vector<Point2f> scene_corners(4);
        
            perspectiveTransform( obj_corners, scene_corners, H);
        
            //-- Draw lines between the corners (the mapped object in the scene - image_2 )
            line( img_matches, scene_corners[0] + Point2f( _srcImg.cols, 0), scene_corners[1] + Point2f( _srcImg.cols, 0), Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1] + Point2f( _srcImg.cols, 0), scene_corners[2] + Point2f( _srcImg.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2] + Point2f( _srcImg.cols, 0), scene_corners[3] + Point2f( _srcImg.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3] + Point2f( _srcImg.cols, 0), scene_corners[0] + Point2f( _srcImg.cols, 0), Scalar( 0, 255, 0), 4 );
            circle(img_matches,points[0] + Point2f( _srcImg.cols, 0), 4, cv::Scalar(255, 100, 100));
            imshow( "matches", img_matches );
            //  usleep( 5 * 1000 );
        }
    }
    else
    {
       // printf("Not enough matches (%d) for homography...\n", (int)mpts_1.size());
        return false;
    }
    
    return true;
}
