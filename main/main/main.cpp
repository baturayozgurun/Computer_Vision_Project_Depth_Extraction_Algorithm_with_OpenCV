//  Developer: Baturay Özgürün
//  Copyright © 2018 Baturay Özgürün
//  If you have any requests or questions please do not hesitate to contact me via baturay.ozgurun@gmail.com

//  Project Name: Computer Vision Project -- Depth Extraction Algorithm with OpenCV
//  Project Desciption: This algorithm aims at estimating depth maps from stereo images by capitalizing on SIFT features. It also presents the feature coordinates of each stereo image and their best matches in text files.

//Include Headers
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;

RNG rng;
double pi = 3.14;
double square(int x){return x*x;}

int main(){
    //Define variables
    Mat ImageL, ImageR, GryImageL, GryImageR, FeatureImageL,FeatureImageR, DescriptorsL,DescriptorsR, ImgMatches,ImgBestMatches;
    Mat RectifiedL,RectifiedR,HomographyL,HomographyR,Disparity,ColorfulDisparity;;
    
    vector<KeyPoint> KeypointsL,KeypointsR;
    vector<Point2f> ImagePointsL,ImagePointsR;
    vector<DMatch> Matches,BestMatches;
    vector<uchar> Status;
    
    int DisparityNumber = 160,WindowSize = 9;
    double minVal,maxVal,max_dist=0,min_dist=100;
    
    //Extract the features for the left and right images
    ImageL=imread("data/left.jpg"); //Read the left image
    cvtColor(ImageL,GryImageL,CV_BGR2GRAY); //Convert the image into gray scale
    Ptr<Feature2D> f2dL = xfeatures2d::SIFT::create(); //Allocate the SIFT feature
    f2dL->detect(GryImageL,KeypointsL); //Calculate features on the gray scale left image and stores in KeypointsL
    cv::drawKeypoints(GryImageL,KeypointsL,FeatureImageL); //Draw the calculated features onto an image named FeatureImageL
    f2dL->compute(GryImageL,KeypointsL,DescriptorsL); //Calculate descpritors for the left image
   
    ImageR=imread("data/right.jpg"); //Read the right image
    cvtColor(ImageR,GryImageR,CV_BGR2GRAY); //Convert the image into gray scale
    Ptr<Feature2D> f2dR = xfeatures2d::SIFT::create(); //Allocate the SIFT feature
    f2dR->detect(GryImageR,KeypointsR); //Calculate features on the gray scale right image and stores in KeypointsR
    cv::drawKeypoints(GryImageR,KeypointsR,FeatureImageR); //Draw the calculated features onto an image named FeatureImageR
    f2dR->compute(GryImageR,KeypointsR,DescriptorsR); //Calculate descpritors for the right image
    
    //Show the number of features for each image
    cout << "The number of features on the left image is: " << KeypointsL.size() << endl;
    cout << "The number of features on the right image is: " << KeypointsR.size() << endl;
    cout << "The feature coordinates of the left and right images are stored in the text files" << endl;

    //Store the feature coordinates in a vector
    vector<Point2f> PointL,PointR;
    for(int i=0;i<KeypointsL.size();i++){
        PointL.push_back(KeypointsL[i].pt);
    }
    for(int i=0;i<KeypointsR.size();i++){
        PointR.push_back(KeypointsR[i].pt);
    }
    
    //Write the feature coordinates in a text file
    FileStorage fsL("results/The Feature Coordinates of the Left Image.txt", FileStorage::WRITE);
    FileStorage fsR("results/The Feature Coordinates of the Right Image.txt", FileStorage::WRITE);
    write(fsL,"Feature coordinates of the left image - Odd and even indices are x and y coordinates respectively",PointL);
    write(fsR,"Feature coordinates of the right image - Odd and even indices are x and y coordinates respectively",PointR);
    fsL.release(); fsR.release();
    
    // Construct The Matcher based on Fast Library for Approximate Nearest Neighbors
    FlannBasedMatcher matcher;
    matcher.match(DescriptorsL,DescriptorsR,Matches);
    drawMatches(GryImageL,KeypointsL,GryImageR,KeypointsR,Matches,ImgMatches); //Draw the computed feature matches
    
    //Calculation of Maximum and Minimum Distances Between Keypoints
    for( int i = 0; i < Matches.size(); i++ ){
        double dist = Matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    
    //Find the best feature matches based on almost five fold minimum distance of the features and store the Image Points
    for(int i=0;i<Matches.size();i++){
        if(Matches[i].distance<=max(4.5*min_dist,0.02)){
            BestMatches.push_back(Matches[i]);
            ImagePointsL.push_back(KeypointsL[Matches[i].queryIdx].pt);
            ImagePointsR.push_back(KeypointsR[Matches[i].trainIdx].pt);
        }
    }
    drawMatches(GryImageL,KeypointsL,GryImageR,KeypointsR,BestMatches,ImgBestMatches);  //Draw the best feature matches
    
    //Calculate the Fundamental Matrix F
    Mat F = findFundamentalMat(ImagePointsL,ImagePointsR,FM_RANSAC,3,0.99,Status);
    stereoRectifyUncalibrated(ImagePointsL,ImagePointsR,F,GryImageL.size(),HomographyL,HomographyR); //Calculate the homography matrices for the left and right images
    
    //Calculate Depth
    warpPerspective(GryImageL, RectifiedL, HomographyL, GryImageL.size()); //Rectify the left image
    warpPerspective(GryImageR, RectifiedR, HomographyR, GryImageR.size()); //Rectify the right image
    Ptr<StereoBM> stereo = StereoBM::create(DisparityNumber,WindowSize); //Define the stereo object
    stereo->compute(RectifiedL,RectifiedR,Disparity); //Calculate disparity or depth
    minMaxLoc(Disparity,&minVal,&maxVal); //Find the maximum and minimum disparity values
    Disparity.convertTo(Disparity,CV_8UC1,255/(maxVal-minVal)); //Make depth map into 8 bit scale ot gray level scale
    applyColorMap(Disparity,ColorfulDisparity,COLORMAP_JET); //Color the depth map
    
    //Show and write all images upto know
    imshow("Gray Scale Left Image",GryImageL);
    imshow("Gray Scale Right Image",GryImageR);
    
    imshow("SIFT Features on the Left Image",FeatureImageL);
    imwrite("results/SIFT Features on the Left Image.jpg",FeatureImageL);
    imshow("SIFT Features on the Right Image",FeatureImageR);
    imwrite("results/SIFT Features on the Right Image.jpg",FeatureImageR);
    
    imshow("Features with the Matches of with Left and Right Images",ImgMatches);
    imwrite("results/Features with the Matches of Left and Right Images.jpg",ImgMatches);
    imshow("Features with the Best Matches of Left and Right Images", ImgBestMatches);
    imwrite("results/Features with the Best Matches of Left and Right Images.jpg",ImgBestMatches);
    
    imshow("Rectified Left Image",RectifiedL);
    imwrite("results/Rectified Left Image.jpg",RectifiedL);
    imshow("Rectified Right Image",RectifiedR);
    imwrite("results/Rectified Right Image.jpg",RectifiedR);
    
    imshow("Disparity Map",Disparity);
    imwrite("results/Disparity Map.jpg",Disparity);
    imshow("Colorful Disparity Map",ColorfulDisparity);
    imwrite("results/Colorful Disparity Map.jpg",ColorfulDisparity);
    
    Mat Blank=Mat::zeros(ImageR.rows,ImageR.cols,ImageR.type()); //Define blank image
    
    //Draw vector field onto the original and blank images
    vector<Point2f> MatchingCoordinatesL,MatchingCoordinatesR;
    for(int i=0;i<BestMatches.size();i++){
        MatchingCoordinatesL.push_back(KeypointsL[BestMatches[i].queryIdx].pt);
        MatchingCoordinatesR.push_back(KeypointsR[BestMatches[i].trainIdx].pt);
        
        //Random value generator for the colorful vector representations
        int color1 = rng.uniform((double)0,(double)255);
        int color2 = rng.uniform((double)0,(double)255);
        int color3 = rng.uniform((double)0,(double)255);
        
        int Line_Thickness = 1;
        Point p,q;
        p.x= (int) MatchingCoordinatesL[i].x;
        p.y= (int) MatchingCoordinatesL[i].y;
        q.x= (int) MatchingCoordinatesR[i].x;
        q.y= (int) MatchingCoordinatesR[i].y;
        double angle=atan2((double)p.y-q.y,(double)p.x-q.x);
        double hypotenuse=sqrt(square(p.y-q.y)+square(p.x-q.x));
        
        q.x=(int)(p.x-hypotenuse*cos(angle));
        q.y=(int)(p.y-hypotenuse*sin(angle));
        line(ImageR,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
        line(Blank,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
        
        //Draw the tips of the arrow
        p.x=(int)(q.x+9*cos(angle+pi/4));
        p.y=(int)(q.y+9*sin(angle+pi/4));
        line(ImageR,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
        line(Blank,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
        p.x=(int)(q.x+9*cos(angle-pi/4));
        p.y=(int)(q.y+9*sin(angle-pi/4));
        line(ImageR,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
        line(Blank,p,q,Scalar(color1,color2,color3),Line_Thickness,CV_AA,0);
    }
    
    cout << "The feature coordinates with the best matches of the left and right images are stored in the text files" << endl << endl;
    //Write the feature coordinates in a text file
    FileStorage MatchingL("results/The Feature Coordinates with the Best Matches of the Left Image.txt", FileStorage::WRITE);
    FileStorage MatchingR("results/The Feature Coordinates with the Best Matches of the Right Image.txt", FileStorage::WRITE);
    write(MatchingL,"The Feature Coordinates with the Best Matches of the Left Image - Odd and even indices are x and y coordinates respectively",MatchingCoordinatesL);
    write(MatchingR,"The Feature Coordinates with the Best Matches of the Right Image - Odd and even indices are x and y coordinates respectively",MatchingCoordinatesR);
    MatchingL.release(); MatchingR.release();
    
    imshow("The Vector Field on the Original Right Image",ImageR);
    imwrite("results/The Vector Field on the Original Right Image.jpg",ImageR);
    imshow("The Vector Field on the Blank Image",Blank);
    imwrite("results/The Vector Field on the Blank Image.jpg",Blank);

    waitKey(1000); //wait to see images
    return 0;
}
