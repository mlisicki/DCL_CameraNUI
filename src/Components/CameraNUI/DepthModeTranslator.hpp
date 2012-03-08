/* 
 * File:   DepthModeTranslator.hpp
 * Author: Michal Lisicki
 */

#ifndef DEPTHMODETRANSLATOR_HPP_
#define	DEPTHMODETRANSLATOR_HPP_

#include <string>

enum DepthMode {
    rawMap,
    normalized,
    disparityMap,
    dM32f,
    pointCloud,
    valid
};

class DepthModeTranslator {
public:
    
    static DepthMode fromStr(const std::string & s);

    static std::string toStr(DepthMode t);
};

#endif	/* DEPTHMODETRANSLATOR_H */

