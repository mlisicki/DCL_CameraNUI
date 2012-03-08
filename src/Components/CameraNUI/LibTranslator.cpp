/* 
 * File:   LibTranslator.cpp
 * Author: Michal Lisicki
 */

#include "LibTranslator.hpp"

Lib LibTranslator::fromStr(const std::string & s) {
    if (s == "freenect")
        return freenect;
    else
        return opencv;
}

std::string LibTranslator::toStr(Lib t) {
    switch(t) {
        case freenect: return "freenect";
        default: return "opencv";
    }
}