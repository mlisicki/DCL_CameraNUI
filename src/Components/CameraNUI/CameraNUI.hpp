/*!
* \file
* \brief
*/

#ifndef CAMERANUI_HPP_
#define CAMERANUI_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "conf.hpp"
#ifdef WITH_OPENNI
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#else
#include <cv.h>
#endif
#include "FreenectNUIDevice.hpp"
#include "LibTranslator.hpp"
#include "DepthModeTranslator.hpp"
#include "CameraModeTranslator.hpp"
#include <XnCppWrapper.h>

namespace Processors {
namespace CameraNUI {

/*!
* \class CameraNUI
* \brief CameraNUI processor class.
*/
class CameraNUI: public Base::Component
{
public:
        /*!
        * Constructor.
        */
        CameraNUI(const std::string & name = "");

        /*!
        * Destructor
        */
        virtual ~CameraNUI();

protected:

        /*!
        * Connects source to given device.
        */
        bool onInit();

        /*!
        * Disconnect source from device, closes streams, etc.
        */
        bool onFinish();

        /*!
        * Retrieves data from device.
        */
        bool onStep();

        /*!
        * Start component
        */
        bool onStart();

        /*!
        * Stop component
        */
        bool onStop();

        void convertToGray(cv::Mat& data, cv::Mat& dataOut);
        void convertToDisparityMap(cv::Mat& data, cv::Mat& dataOut);
        void convertToDisparityMap32f(cv::Mat& data, cv::Mat& dataOut);
        void convertToPointCloudMap(cv::Mat& data, cv::Mat& dataOut);
        void convertToValidPixelsMap(cv::Mat& data, cv::Mat& dataOut);
        
        // depth map normalization factor
        static const float SCALE_FACTOR = 255.0/2048.0;
        // openKinect documentation:
        // "When the Kinect can't see the ir reflection 
        // or has no depth data for a pixel, it returns 
        // 2047 for the depth value"
        static const int INVALID_PIXEL = 2047;
        static const int INVALID_COORDINATE = 0;
        // baseline and focal length from opencv properties
        static const int BASELINE = 75;
        static const int FOCAL_LENGTH = 575;
        // for now only this resolution fully supported
        static const int COLS = 640;
        static const int ROWS = 480;
        
        Base::Property<bool> skipStop;
        Base::Property<int,DepthModeTranslator> depthMode;  
        Base::Property<int,CameraModeTranslator> cameraMode; 
        Base::Property<int,LibTranslator> lib;
        
        Base::DataStreamOut <cv::Mat> outImg;
        Base::DataStreamOut <cv::Mat> outDepthMap;
        
        Base::Event* newImage;
        Base::Event* newDepthMap;
        
        Freenect::Freenect freenectObj;
        FreenectNUIDevice* device;
        
        cv::VideoCapture capture;        
        
        cv::Mat cameraFrame;
        cv::Mat depthFrame;
        Mat show;
};

}//: namespace CameraNUI
}//: namespace Processors

/*
* Register processor component.
*/
REGISTER_PROCESSOR_COMPONENT("CameraNUI", Processors::CameraNUI::CameraNUI, Common::Panel_Empty)

#endif /* CAMERANUI_HPP_ */