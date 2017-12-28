//
// Created by xiangdong on 2017/12/21.
//

#ifndef XHTMLPARSE_TOOL_H
#define XHTMLPARSE_TOOL_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <sys/time.h>
#include <algorithm>
#include <stdlib.h>
#include <cstring>
#include <regex>
#include <map>

using namespace std;
void string_replace(std::string &s1, const std::string &s2, const std::string &s3);
std::string& trim(std::string &s);
std::size_t xStrspn(std::string bStr,std::string sStr,std::size_t pos=0);

#endif //XHTMLPARSE_TOOL_H
