//
// Created by xiangdong on 2017/12/21.
//

#ifndef XHTMLPARSE_TREE_H
#define XHTMLPARSE_TREE_H

#include "Tool.h"
#include "Node.h"

using namespace std;

class Tree {
public:
    vector<class Node> nodes;
    std::string nowChar;
    std::size_t pos;
    std::size_t size;
    std::size_t original_size;
    std::size_t cursor; //当前节点id
    std::string doc;
    /**
     * 选择性关闭标签
     */
    map<std::string, map<std::string, int> > optionalClosingTags;
    /**
     *块标签
     */
    map<std::string, int> blockTags;
    map<std::string, int> selfCloseTags;

    class Node *root;

    class Node *parent;

    //dom类型
    enum {
        t_element = 1, t_comment, t_text, t_endtag, t_root, t_unknow
    };
    //内容类型
    enum {
        t_begin = 0,
        t_end,
        t_quote,
        t_space,
        t_inner,
        t_outer,
        t_endspace
    };
    std::string token_blank = " \t\r\n";
    std::string token_slash = " />\r\n\t";
    std::string token_equal = " =/>";
    std::string token_attr = ">";

    static map<std::string, int> createBlockTags() {
        map<std::string, int> reMap;
        reMap["root"] = 1;
        reMap["body"] = 1;
        reMap["form"] = 1;
        reMap["div"] = 1;
        reMap["span"] = 1;
        reMap["table"] = 1;
        reMap["video"] = 1;
        reMap["source"] = 1;
        reMap["code"] = 1;
        reMap["pre"] = 1;
        return reMap;
    }

    static map<std::string, int> createSelfCloseTags() {
        map<std::string, int> reMap;
        reMap["img"] = 1;
        reMap["br"] = 1;
        reMap["input"] = 1;
        reMap["meta"] = 1;
        reMap["link"] = 1;
        reMap["hr"] = 1;
        reMap["base"] = 1;
        reMap["embed"] = 1;
        reMap["spacer"] = 1;
        return reMap;
    }

    static map<std::string, map<std::string, int> > createOptionalClosingTags() {
        map<std::string, map<std::string, int> > reMap;
        map<std::string, int> tMap;
        tMap["tr"] = 1;
        tMap["td"] = 1;
        tMap["th"] = 1;
        reMap["tr"] = tMap;
        tMap.clear();
        tMap["th"] = 1;
        reMap["th"] = tMap;
        tMap.clear();
        tMap["td"] = 1;
        reMap["td"] = tMap;
        tMap.clear();
        tMap["li"] = 1;
        reMap["li"] = tMap;
        tMap.clear();
        tMap["dt"] = 1;
        tMap["dd"] = 1;
        reMap["dt"] = tMap;
        tMap.clear();
        tMap["dd"] = 1;
        tMap["dt"] = 1;
        reMap["dd"] = tMap;
        tMap.clear();
        tMap["dd"] = 1;
        tMap["dt"] = 1;
        reMap["dl"] = tMap;
        tMap.clear();
        tMap["p"] = 1;
        reMap["p"] = tMap;
        tMap.clear();
        tMap["nobr"] = 1;
        reMap["nobr"] = tMap;
        tMap.clear();
        tMap["b"] = 1;
        reMap["b"] = tMap;
        tMap.clear();
        tMap["option"] = 1;
        reMap["option"] = tMap;
        return reMap;
    }

    std::string copyUntilChar(const std::string str);

    std::string copyUntil(const std::string str);

    std::string copySkip(const std::string str);

    std::string copyUntilCharEscape(const std::string str);

    Tree(void);

    void skip(std::string str);

    bool readTag();

    void parseAttr(Node node, std::string name, map<int, std::string> &space);

    bool asTextNode(std::string str);

    void linkNodes(class Node &node, bool isChild);


    std::string nextChar();

    std::string preChar();

    void prepare(std::string str);

    Tree *load(std::string);

    bool parse();

    void dump(bool isShowAttr = true);
};

#endif //XHTMLPARSE_TREE_H
