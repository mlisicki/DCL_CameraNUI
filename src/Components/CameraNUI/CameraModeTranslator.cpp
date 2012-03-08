/* 
 * File:   CameraModeTranslator.cpp
 * Author: Michal Lisicki
 */


#include "CameraModeTranslator.hpp"

CameraMode CameraModeTranslator::fromStr(const std::string & s) {
    if (s == "gray")
        return gray;
    else
        return bgr;
}

std::string CameraModeTranslator::toStr(CameraMode t) {
    switch(t) {
        case gray: return "gray";
        default: return "bgr";
    }
}  