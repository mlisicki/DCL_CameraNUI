/* 
 * File:   FreenectNUIDevice.cpp
 * Author: mike
 * 
 * Created on March 2, 2012, 12:34 AM
 */

#include "FreenectNUIDevice.hpp"
#include "DepthModeTranslator.hpp"

using namespace cv;
using namespace std;

FreenectNUIDevice::FreenectNUIDevice(freenect_context *_ctx, int _index)
        : Freenect::FreenectDevice(_ctx, _index), m_new_rgb_frame(false), m_new_depth_frame(false),
            depthMat(Size(640,480),CV_16UC1), rgbMat(Size(640,480),CV_8UC3,Scalar(0))
{
}

void FreenectNUIDevice::VideoCallback(void* _rgb, uint32_t timestamp) {
    pthread_mutex_lock(&m_rgb_mutex);
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    rgbMat.data = rgb;
    m_new_rgb_frame = true;
    pthread_mutex_unlock(&m_rgb_mutex);       
}

void FreenectNUIDevice::DepthCallback(void* _depth, uint32_t timestamp) {
    pthread_mutex_lock(&m_depth_mutex);
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    depthMat.data = (uchar*) depth;
    m_new_depth_frame = true;
    pthread_mutex_unlock(&m_depth_mutex);    
}

bool FreenectNUIDevice::getVideo(Mat& output) {
    pthread_mutex_lock(&m_rgb_mutex);
    if(m_new_rgb_frame) {
        cv::cvtColor(rgbMat, output, CV_RGB2BGR);
        m_new_rgb_frame = false;
        pthread_mutex_unlock(&m_rgb_mutex);
        return true;
    } else {
        pthread_mutex_unlock(&m_rgb_mutex);
        return false;
    }    
}

bool FreenectNUIDevice::getDepth(Mat& output) {
    pthread_mutex_lock(&m_depth_mutex);
    if(m_new_depth_frame) {
        depthMat.copyTo(output);
        m_new_depth_frame = false;
        pthread_mutex_unlock(&m_depth_mutex);
        return true;
    } else {
        pthread_mutex_unlock(&m_depth_mutex);
        return false;
    }
}
