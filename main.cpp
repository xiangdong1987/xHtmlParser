//
//  main.cpp
//  xHtmlParse
//
//  Created by xiangdong on 2017/12/25.
//  Copyright © 2017年 xiangdong. All rights reserved.
//

#include <iostream>
#include "Tree.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    Tree tree;
    std::string xdd= "123232<div>xdd</div><font>123</font>";
    tree.load(xdd);
    tree.dump(true);

    return 0;
}
