/* 
 * File:   CameraModeTranslator.hpp
 * Author: Michal Lisicki
 */

#ifndef CAMERAMODETRANSLATOR_H
#define	CAMERAMODETRANSLATOR_H

#include <string>

enum CameraMode {
    bgr,
    gray,
};

class CameraModeTranslator {
public:
    
    static CameraMode fromStr(const std::string & s);

    static std::string toStr(CameraMode t);
};

#endif	/* CAMERAMODETRANSLATOR_H */

