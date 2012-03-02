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

CameraNUI::CameraNUI(const std::string & name) : Base::Component(name) {
	LOG(LTRACE) << "Hello CameraNUI\n";
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

        device = &freenect.createDevice<FreenectNUIDevice>(0);
        depthf = Mat(Size(640,480),CV_8UC1);
        rgbFrame = Mat(Size(640,480),CV_8UC3,Scalar(0));
        depthFrame = Mat(Size(640,480),CV_16UC1);

        return true;
}

bool CameraNUI::onFinish() {
	LOG(LTRACE) << "CameraNUI::finish\n";
	return true;
}

bool CameraNUI::onStep() {
        Mat rgbFrameCopy;
        Mat depthFrameCopy;
        Mat frame;
	LOG(LTRACE) << "CameraNUI::step\n";
        device->getVideo(rgbFrame);
    	device->getDepth(depthFrame);
        // Write rgb frame to output data stream
        rgbFrame.copyTo(rgbFrameCopy);
        outImg.write(rgbFrameCopy);
        // Write depth map to output data stream
        depthFrame.convertTo(depthf, CV_8UC1, 255.0/2048.0);
        depthf.copyTo(depthFrameCopy);
        outDepthMap.write(depthFrameCopy);
        //Raise events
        newImage->raise();
        newDepthMap->raise();
        ///cvWaitKey(5);
	return true;
}

bool CameraNUI::onStop() {
	device->stopVideo();
	device->stopDepth();
	return true;
}

bool CameraNUI::onStart() {
	device->startVideo();
	device->startDepth();
	return true;
}

}//: namespace CameraNUI
}//: namespace Processors
