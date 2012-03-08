/* 
 * File:   LibTranslator.hpp
 * Author: Michal Lisicki
 */

#ifndef LIBTRANSLATOR_HPP_
#define	LIBTRANSLATOR_HPP_

#include <string>

enum Lib {
    freenect,
    opencv,
};

class LibTranslator {
public:
    
    static Lib fromStr(const std::string & s);

    static std::string toStr(Lib t);
};

#endif	/* LIBTRANSLATOR_HPP_ */

