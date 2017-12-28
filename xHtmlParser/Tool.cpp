//
// Created by xiangdong on 2017/12/21.
//
#include "Tool.h"

void string_replace(std::string &s1, const std::string &s2, const std::string &s3) {
    std::size_t pos = 0;
    std::size_t a = s2.size();
    std::size_t b = s3.size();
    while ((pos = s1.find(s2, pos)) != std::string::npos) {
        s1.replace(pos, a, s3);
        pos += b;
    }
}

std::string &trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

std::size_t xStrspn(std::string bStr, std::string sStr, std::size_t pos) {
    std::size_t len;
    std::string tStr;
    if (pos > 0) {
        tStr = bStr.substr(pos);
    } else {
        tStr = bStr;
    }
    for (len = 0; len <= tStr.length(); len++) {
        std::string oneStr = tStr.substr(len, 1);
        if (sStr.find(oneStr) == std::string::npos) {
            continue;
        } else {
            break;
        }
    }
    if (len == tStr.length()) {
        return 0;
    }
    return len;
}