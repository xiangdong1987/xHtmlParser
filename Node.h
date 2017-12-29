//
// Created by xiangdong on 2017/12/21.
//

#ifndef XHTMLPARSE_NODE_H
#define XHTMLPARSE_NODE_H

#include "Tree.h"

using namespace std;

/**
 * 定义节点结构体
 */
struct xNode {
    xNode *parent;
    vector<xNode *> nodes;
    vector<xNode *> children;
    std::string tag = "text";
    map<int, std::string> _;
    std::size_t tag_start;
    std::size_t endCursor;
    map<std::string, std::string> attr;

    void dump(bool isShowAttr = true, int deep = 0);

    class Tree *dom;

    int nodetype;
};

class Node {
public:
    std::string tag = "text";
    map<int, std::string> _;
    std::size_t endCursor;
    map<std::string, std::string> attr;

    class Node *parent;

    vector<class Node> nodes;
    vector<class Node> children;
    std::size_t tag_start;

    class Tree *dom = nullptr;

    int nodetype;

    Node(Tree *dom);

    void dump(bool isShowAttr = true, int deep = 0);
};

#endif //XHTMLPARSE_NODE_H
