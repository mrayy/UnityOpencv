//
//  Utilities.hpp
//  UnityOpenCV
//
//  Created by MHD Yamen Saraiji on 12/28/16.
//
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <stdio.h>

#include <string>
#include <vector>
#include <sstream>


void split(const std::string &s, char delim, std::vector<std::string> &elems) ;
std::vector<std::string> split(const std::string &s, char delim);

std::string &trim(std::string &s);

#endif /* Utilities_hpp */
