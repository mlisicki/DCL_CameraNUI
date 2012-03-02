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
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "FreenectNUIDevice.hpp"

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
        
        Base::DataStreamOut <cv::Mat> outImg;
        Base::DataStreamOut <cv::Mat> outDepthMap;
        
        Base::Event* newImage;
        Base::Event* newDepthMap;
        
        Freenect::Freenect freenect;
        FreenectNUIDevice* device;
        
        cv::Mat depthf;
        cv::Mat rgbFrame;
        cv::Mat depthFrame;
};

}//: namespace CameraNUI
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CameraNUI", Processors::CameraNUI::CameraNUI, Common::Panel_Empty)

#endif /* CAMERANUI_HPP_ */
