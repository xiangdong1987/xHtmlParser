//
// Created by xiangdong on 2017/12/21.
//
#include "Node.h"

Node::Node(Tree *dom) {
    this->dom = dom;
    this->dom->nodes.push_back(*this);
}

void Node::dump(bool isShowAttr, int deep) {
    std::string lead;
    std::string tab = "    ";
    int i;
    for (i = 0; i <= deep; i++) {
        lead += tab;
    }
    cout << lead + this->tag;
    if (isShowAttr && !this->attr.empty()) {
        cout << "(";
        map<std::string, std::string>::iterator iter;
        iter = this->attr.begin();
        while (iter != this->attr.end()) {
            cout << "[" << iter->first << "]=>" << iter->second;
            iter++;
        }
        cout << ")";
    }
    cout << "\n";
    if (this->nodes.size() > 0) {
        long count = this->nodes.size();
        for (long i = 0; i < count; i++) {
            this->nodes[i].dump(isShowAttr, deep + 1);
        }
    }
}