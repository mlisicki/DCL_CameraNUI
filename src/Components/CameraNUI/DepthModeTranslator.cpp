/* 
 * File:   DepthModeTranslator.cpp
 * Author: Michal Lisicki
 */

#include "DepthModeTranslator.hpp"

DepthMode DepthModeTranslator::fromStr(const std::string & s) {
    if (s == "raw_map")
        return rawMap;
    else if (s == "disparity_map")
        return disparityMap;
    else if (s == "dm32f")
        return dM32f;
    else if (s == "point_cloud")
        return pointCloud;
    else if (s == "valid")
        return valid;    
    else
        return normalized;
}

std::string DepthModeTranslator::toStr(DepthMode t) {
    switch(t) {
        case rawMap: return "raw_map";
        case normalized: return "normalized";
        case disparityMap: return "disparity_map";
        case dM32f: return "dm32f";
        case pointCloud: return "point_cloud";
        default: return "normalized";
    }
}