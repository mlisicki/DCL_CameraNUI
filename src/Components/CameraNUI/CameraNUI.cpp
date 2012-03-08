/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CameraNUI.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CameraNUI {

CameraNUI::CameraNUI(const std::string & name) : Base::Component(name), 
        skipStop("skip_stop",false),
        lib("lib",freenect),
        depthMode("depth_mode",rawMap),
        cameraMode("camera_mode",bgr) {

        LOG(LTRACE) << "Hello CameraNUI\n";
        registerProperty(skipStop);
        registerProperty(lib);
        registerProperty(depthMode);
        registerProperty(cameraMode);
}

CameraNUI::~CameraNUI() {
	LOG(LTRACE) << "Good bye CameraNUI\n";
}

bool CameraNUI::onInit() {
	LOG(LTRACE) << "CameraNUI::initialize\n";
        
        // Regsiter events
        newImage = registerEvent("newImage");
        newDepthMap = registerEvent("newDepthMap");
        
        // Register data streams
        registerStream("out_img", &outImg);
        registerStream("out_depth_map", &outDepthMap);
//               
#ifdef WITH_OPENNI        
        if(lib == opencv) {
                capture.open( CV_CAP_OPENNI );
                std::cout << "Yes. We're here\n";
        } else {
#endif
        device = &freenectObj.createDevice<FreenectNUIDevice>(0);
#ifdef WITH_OPENNI 
        }
#endif
        cameraFrame = Mat(Size(640,480),CV_8UC3,Scalar(0));
        depthFrame = Mat(Size(640,480),CV_16UC1);
        return true;
}

bool CameraNUI::onFinish() {
	LOG(LTRACE) << "CameraNUI::finish\n";
	return true;
}

bool CameraNUI::onStep() {
        try {
            Mat cameraFrameCopy;
            Mat depthFrameCopy;
            Mat frame;
            LOG(LTRACE) << "CameraNUI::step\n";
#ifdef WITH_OPENNI
            if(lib == opencv) {
                if( !capture.grab() ) {
                    LOG(LERROR) << "CameraKinect::onStep: Can't grab images\n";
                } else {
                    if(depthMode == normalized) {
                        if( capture.retrieve( depthFrame, CV_CAP_OPENNI_DEPTH_MAP ) ) {
                            depthFrame.convertTo( show, CV_8UC1, 0.05 );
                            show.copyTo(depthFrameCopy);
                        }
                    } else if(depthMode == disparityMap) {
                        capture.retrieve( depthFrame, CV_CAP_OPENNI_VALID_DEPTH_MASK );
                        depthFrame.copyTo(depthFrameCopy);
                    } else if(depthMode == dM32f) {
                        capture.retrieve( depthFrame, CV_CAP_OPENNI_DISPARITY_MAP_32F );
                        depthFrame.copyTo(depthFrameCopy);
                    } else if(depthMode == pointCloud) {
                        capture.retrieve( depthFrame, CV_CAP_OPENNI_POINT_CLOUD_MAP );
                        depthFrame.copyTo(depthFrameCopy);
                    } else if(depthMode == valid) {
                        capture.retrieve( depthFrame, CV_CAP_OPENNI_VALID_DEPTH_MASK );
                        depthFrame.copyTo(depthFrameCopy);
                    } else {
                        depthFrame.copyTo(depthFrameCopy);   
                    }
                    
                    if(cameraMode == gray) {
                        capture.retrieve( cameraFrame, CV_CAP_OPENNI_GRAY_IMAGE );
                        cameraFrame.copyTo(cameraFrameCopy);
                    } else {
                        capture.retrieve( cameraFrame, CV_CAP_OPENNI_BGR_IMAGE );
                        cameraFrame.copyTo(cameraFrameCopy);
                    }
                }             
            } else {
#endif
                device->getVideo(cameraFrame);
                device->getDepth(depthFrame);
                // Write camera frame to output data stream
                // Write depth map to output data stream
                if(depthMode == normalized) {
                        depthFrame.convertTo( show, CV_8UC1, SCALE_FACTOR );
                        show.copyTo(depthFrameCopy);
                } else if(depthMode == dM32f) {
                        convertToDisparityMap32f(depthFrame,depthFrameCopy);
                } else if(depthMode == disparityMap) {
                        convertToDisparityMap(depthFrame,depthFrameCopy);
                } else if(depthMode == pointCloud) {
                        convertToPointCloudMap(depthFrame,depthFrameCopy);
                } else if(depthMode == valid) {
                        convertToValidPixelsMap(depthFrame,depthFrameCopy);
                } else {
                        depthFrame.copyTo(depthFrameCopy);   
                }
                
                if(cameraMode == gray) {
                    convertToGray( cameraFrame, cameraFrameCopy );
                } else {
                    cameraFrame.copyTo(cameraFrameCopy);
                }    
#ifdef WITH_OPENNI
            }
#endif
            outImg.write(cameraFrameCopy);           
            outDepthMap.write(depthFrameCopy);
            //Raise events
            newImage->raise();
            newDepthMap->raise();
        } catch (...) {
            LOG(LERROR) << "CameraNUI::onStep failed\n";
        }
	return true;
}

bool CameraNUI::onStop() {
        if(!(skipStop || lib == opencv)) {
                device->stopVideo();
                device->stopDepth();
        }
	return true;
}

bool CameraNUI::onStart() {  
#ifdef WITH_OPENNI
        if(lib == opencv) {
            if(!capture.isOpened()) {
                LOG(LERROR) << "CameraNUI::onStart: Can't access Kinect sensor\n";
                return false;
            }
            // check baseline and focal length
//            double bl = capture.get( CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE );
//            LOG(LINFO) << "baseline = " << bl << std::endl;
//            double fl = capture.get( CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH );
//            LOG(LINFO) << "focal length = " << fl << std::endl; 
        } else {
#endif
        device->startVideo();
	device->startDepth();          
#ifdef WITH_OPENNI 
        }
#endif
	return true;
}

void CameraNUI::convertToGray(cv::Mat& data, cv::Mat& dataOut) {
    cv::cvtColor(data, dataOut, CV_BGR2GRAY);
}

/*
 * Based on OpenCv HighGUI
 */
void CameraNUI::convertToPointCloudMap(cv::Mat& data, cv::Mat& dataOut) {
    // !! This data are not exactly the same as when using opencv. Might need
    // some adjustments in the future. It's supposed to imitate the function
    // xnConvertProjectiveToRealWorld from OpenNI. Some attempt for this
    // was made in discussion on google groups 
    // "ConvertProjectiveToRealWorld without camera connected Indstillinger"
    // To move further make use of content in the file cap_openni.cpp from OpenCv
    // and XnOpenNI.cpp from OpenNI
    // The whole idea behind is to transform coordinates and have them in real
    // world units (like meters)
    double fx_d = 1.0 / 5.9421434211923247e+02;
    double fy_d = 1.0 / 5.9104053696870778e+02;
    double cx_d = 3.3930780975300314e+02;
    double cy_d = 2.4273913761751615e+02;     
       
    cv::Mat pointCloud_XYZ( ROWS, COLS, CV_32FC3, cv::Scalar::all(INVALID_PIXEL) );

    for( int y = 0; y < ROWS; y++ ) {
        for( int x = 0; x < COLS; x++ ) {
            double depth = data.at<unsigned short>(y, x);            
            // Check for invalid measurements
            if( depth == INVALID_PIXEL ) // not valid
                pointCloud_XYZ.at<cv::Point3f>(y,x) = cv::Point3f( INVALID_COORDINATE, INVALID_COORDINATE, INVALID_COORDINATE );
            else {
                pointCloud_XYZ.at<cv::Point3f>(y,x) = cv::Point3f( 
                        ((x - cx_d) * depth * fx_d)*0.001f, 
                        ((y - cy_d) * depth * fy_d)*0.001f, 
                        depth*0.001f);
            }
        }
    }

    pointCloud_XYZ.copyTo(dataOut);
}

/*
 * Based on OpenCv HighGUI
 */
void CameraNUI::convertToDisparityMap(cv::Mat& data, cv::Mat& dataOut) {
    cv::Mat disp32;

    double mult = BASELINE * FOCAL_LENGTH;
    
    disp32.create( data.size(), CV_32FC1);
    disp32 = cv::Scalar::all( INVALID_PIXEL );
    for( int y = 0; y < disp32.rows; y++ ) {
        for( int x = 0; x < disp32.cols; x++ ) {
            unsigned short curDepth = data.at<unsigned short>(y,x);
            if( curDepth != INVALID_PIXEL )
                disp32.at<float>(y,x) = mult / curDepth;
        }
    }
    disp32.convertTo( dataOut, CV_8UC1 );
}

void CameraNUI::convertToDisparityMap32f(cv::Mat& data, cv::Mat& dataOut) {
    double mult = BASELINE * FOCAL_LENGTH;
    
    dataOut.create( data.size(), CV_32FC1);
    dataOut = cv::Scalar::all( INVALID_PIXEL );
    for( int y = 0; y < dataOut.rows; y++ ) {
        for( int x = 0; x < dataOut.cols; x++ ) {
            unsigned short curDepth = data.at<unsigned short>(y,x);
            if( curDepth != INVALID_PIXEL )
                dataOut.at<float>(y,x) = mult / curDepth;
        }
    }
}

void CameraNUI::convertToValidPixelsMap(cv::Mat& data, cv::Mat& dataOut) {
    dataOut = (data != INVALID_PIXEL);
}

}//: namespace CameraNUI
}//: namespace Processors
