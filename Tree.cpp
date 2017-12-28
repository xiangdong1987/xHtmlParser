//
// Created by xiangdong on 2017/12/21.
//

#include "Tree.h"

//初始化类
Tree::Tree() {
    this->optionalClosingTags = this->createOptionalClosingTags();
    this->blockTags = this->createBlockTags();
    this->selfCloseTags = this->createSelfCloseTags();
}

/**
 * 拷贝某个字符串之前的所有数据
 */
std::string Tree::copyUntilChar(const std::string str) {
    std::string rStr;
    if ("" == this->nowChar)
        return "";
    std::size_t f_pos;
    f_pos = this->doc.find(str, this->pos);
    if (f_pos == std::string::npos) {
        rStr = this->doc.substr(this->pos, this->size - this->pos);
        this->nowChar = "";
        this->pos = this->size;
        return rStr;
    }
    if (f_pos == this->pos)
        return "";
    std::size_t pos_old = this->pos;
    this->nowChar = this->doc.substr(f_pos, 1);
    this->pos = f_pos;
    return this->doc.substr(pos_old, f_pos - pos_old);
}

std::string Tree::copyUntil(const std::string str) {
    std::size_t pos = this->pos;
    std::size_t len = xStrspn(this->doc, str, pos);
    this->pos += len;
    this->nowChar = this->doc.substr(this->pos, 1);
    return this->doc.substr(pos, len);

}

std::string Tree::copySkip(const std::string str) {
    std::size_t pos = this->pos;
    std::size_t len = xStrspn(this->doc, str, pos);
    this->pos += len;
    this->nowChar = this->doc.substr(this->pos, 1);
    if (len == 0) return "";
    return this->doc.substr(pos, len);
}

std::string Tree::copyUntilCharEscape(const std::string str) {
    if (this->nowChar == "") { return ""; }
    std::size_t start = this->pos;
    std::size_t pos_t;
    while (1) {
        pos_t = this->doc.find(str, start);
        if (pos_t == std::string::npos) {
            std::string ret = this->doc.substr(this->pos, this->size - this->pos);
            this->nowChar = "";
            this->pos = this->size;
            return ret;
        }
        if (pos_t == this->pos) return "";
        if (this->doc.substr(pos_t - 1, 1) == "\\") {
            start = pos_t + 1;
            continue;
        }
        std::size_t pos_old = this->pos;
        this->nowChar = this->doc.substr(pos_t, 1);
        this->pos = pos_t;
        return this->doc.substr(pos_old, pos_t - pos_old);
    }
}

void Tree::skip(std::string str) {
    this->pos += xStrspn(this->doc, str, this->pos);
    this->nowChar = this->doc.substr(this->pos, 1);
}

bool Tree::readTag() {
    //读取结束
    if (this->nowChar != "<") {
        this->root->_[t_end] = this->cursor;
        return false;
    }
    std::size_t begin_tag_pos = 0;
    this->nowChar = this->nextChar();
    if (this->nowChar == "/") {
        this->nowChar = this->nextChar();
        //跳过换行
        this->skip(this->token_blank);
        std::string tag = this->copyUntilChar(">");
        //取标签名称
        std::size_t skip_pos = tag.find(' ');
        if (skip_pos != std::string::npos) {
            tag = tag.substr(0, skip_pos);
        }
        std::string parentLow;
        parentLow.resize(this->parent->tag.size());
        std::transform(this->parent->tag.begin(), this->parent->tag.end(), parentLow.begin(), ::tolower);
        std::string tagLow;
        tagLow.resize(tag.size());
        std::transform(tag.begin(), tag.end(), tagLow.begin(), ::tolower);
        //判断标签是否与父标签一致
        if (parentLow != tagLow) {
            //父标签是选择性关闭标签并且 当前变标签是块标签
            if (this->optionalClosingTags.count(parentLow) > 0 && this->blockTags.count(tagLow) > 0) {
                this->parent->_[t_end] = "";
                Node *org_parent = this->parent;
                //循环查找父标签 知道找到与当前标签相同的标签
                while ((this->parent->parent) && parentLow != tagLow) {
                    this->parent = this->parent->parent;
                }
                //如果最后的父标签和这个标签不一致 就是个文本
                if (parentLow != tagLow) {
                    this->parent = org_parent;
                    if (this->parent->parent)
                        this->parent = this->parent->parent;
                    this->parent->endCursor = this->cursor;
                    return this->asTextNode(tag);
                }
                //父标签不是选择性关闭标签 并且存在父父节点 ，标签是块标签
            } else if ((this->parent->parent) && this->blockTags.count(tagLow) > 0) {
                //this->parent->_[t_end] = "";
                Node *org_parent = this->parent;

                while ((this->parent->parent) && parentLow != tagLow) {
                    this->parent = this->parent->parent;
                    parentLow.resize(this->parent->tag.size());
                    std::transform(this->parent->tag.begin(), this->parent->tag.end(), parentLow.begin(), ::tolower);
                }
                if (parentLow != tagLow) {
                    this->parent = org_parent;
                    this->parent->endCursor = this->cursor;
                    return this->asTextNode(tag);
                }
            } else if ((this->parent->parent)
                       && (parentLow == tagLow)) {
                this->parent->endCursor = 0;
                this->parent = this->parent->parent;
            } else {
                return this->asTextNode(tag);
            }
        }
        this->parent->endCursor = this->cursor;
        if (this->parent->parent) {
            this->parent = this->parent->parent;
        }
        this->nowChar = this->nextChar();
        return true;
    }
    Node node(this);
    node._[t_begin] = this->cursor;
    ++this->cursor;
    std::string tag = this->copyUntil(this->token_slash);
    node.tag_start = begin_tag_pos;
    //判断是否是注释
    if (tag.length() > 0 && tag.substr(0, 1) == "!") {
        node._[t_text] = "<" + tag + this->copyUntilChar(">");
        if (tag.length() > 3 && tag.substr(1, 1) == "-"
            && tag.substr(2, 1) == "-") {
            node.nodetype = t_comment;
            node.tag = "comment";
        } else {
            node.nodetype = t_unknow;
            node.tag = "unkonw";
        }
        if (this->nowChar == ">") {
            node._[t_text] += ">";
        }
        this->linkNodes(node, true);
        this->nowChar = this->nextChar();
        return true;
    }
    //判断是否是内容
    if (tag.find('<') != std::string::npos) {
        tag = '<' + tag.substr(0, -1);
        node._[t_text] = tag;
        this->linkNodes(node, false);
        this->preChar();
    }
    //空标签
    regex reg("\\w+");
    smatch r;
    if (!regex_match(tag, r, reg)) {
        node._[t_text] = "<" + tag + this->copyUntil("<>");
        if (this->nowChar == "<") {
            this->linkNodes(node, false);
            return true;
        }
        if (this->nowChar == ">") {
            node._[t_text] += '>';
        }
        this->linkNodes(node, false);
        this->nowChar = this->nextChar();
        return true;
    }
    //开始标签
    std::string parentLow;
    parentLow.resize(this->parent->tag.size());
    std::transform(this->parent->tag.begin(), this->parent->tag.end(), parentLow.begin(), ::tolower);
    std::string tagLow;
    tagLow.resize(tag.size());
    std::transform(tag.begin(), tag.end(), tagLow.begin(), ::tolower);
    node.nodetype = t_element;
    std::string tag_lower = tagLow;
    node.tag = tag_lower;
    //处理可选关闭标签
    if (this->optionalClosingTags.count(parentLow) > 0) {
        while (this->optionalClosingTags.count(parentLow) > 0) {
            this->parent->_[t_end] = "";
            this->parent = this->parent->parent;
        }
        node.parent = this->parent;
    }
    //警卫防止死循环
    std::size_t guard = 0;
    map<int, std::string> space;
    space[0] = this->copySkip(this->token_blank);
    space[1] = "";
    space[2] = "";
    //分析标签内元素
    do {
        if (this->nowChar != "" && space[0] == "") {
            break;
        }
        std::string name = this->copyUntil(this->token_equal);
        if (guard == this->pos) {
            this->nowChar = this->nextChar();
            continue;
        }
        guard = this->pos;
        if (this->pos >= this->size - 1 && this->nowChar != ">") {
            node.nodetype = t_text;
            node._[t_end] = "";
            node._[t_text] = "<" + tag + space[0] + name;
            node.tag = "text";
            this->linkNodes(node, false);
            return true;
        }
        if (this->doc.substr(this->pos - 1, 1) == "<") {
            node.nodetype = t_text;
            node.tag = "text";
            node._[t_end] = "";
            node._[t_text] = this->doc.substr(begin_tag_pos, this->pos - begin_tag_pos - 1);
            this->pos -= 2;
            this->nowChar = this->nextChar();
            this->linkNodes(node, false);
            return true;
        }
        if (name != "/" && name != "") {
            space[1] = this->copySkip(this->token_blank);
            //todo 降噪后续在做
            if (this->nowChar == "=") {
                this->nowChar = this->nextChar();
                //解析标签内元素
                this->parseAttr(node, name, space);
            } else {
                node.attr[name] = true;
                if (this->nowChar != ">") {
                    this->nowChar = this->preChar();
                }
            }

        } else {
            break;
        }
    } while (this->nowChar != ">" && this->nowChar != "/");
    this->linkNodes(node, true);
    if (this->copyUntilCharEscape(">") == "/") {
        node._[t_endspace] += "/";
        node._[t_end] = "";
    } else {
        if (this->selfCloseTags.count(tagLow) <= 0) {
            //cout << node.tag << "\n";
            this->parent = &node;
        }
    }
    this->nowChar = this->nextChar();
    if (node.tag == "br") {

    }
    return true;
}

void Tree::parseAttr(Node node, std::string name, map<int, std::string> &space) {
    if (node.attr.count(name) > 0) {
        return;
    }
    space[2] = this->copySkip(this->token_blank);
    if (this->nowChar == "\"") {
        this->nowChar = this->nextChar();
        node.attr[name] = this->copyUntilCharEscape("\"");
        this->nowChar = this->nextChar();
    } else if (this->nowChar == "\'") {
        this->nowChar = this->nextChar();
        node.attr[name] = this->copyUntilCharEscape("\'");
        this->nowChar = this->nextChar();
    } else {
        node.attr[name] = this->copyUntil(this->token_attr);
    }
    string_replace(node.attr[name], "\r", "");
    string_replace(node.attr[name], "\n", "");
    if (name == "class") {
        node.attr[name] = trim(node.attr[name]);
    }
}

bool Tree::asTextNode(std::string str) {
    Node node(this);
    ++this->cursor;
    node._[t_text] = "</" + str + ">";
    return true;
}

void Tree::linkNodes(class Node &node, bool isChild) {
    node.parent = this->parent;
    this->parent->nodes.push_back(node);
    if (isChild) {
        this->parent->children.push_back(node);
    }
}

std::string Tree::nextChar() {
    ++this->pos;
    if (this->pos < this->size) {
        return this->doc.substr(this->pos, 1);
    } else {
        return "";
    }
}

std::string Tree::preChar() {
    --this->pos;
    return this->doc.substr(this->pos, 1);
}

void Tree::prepare(std::string str) {
    this->size = str.length();
    this->original_size = this->size;
    this->doc = str;
    this->pos = 0;
    this->cursor = 1;
    this->root = new Node(this);
    this->root->tag = "root";
    this->parent = this->root;
    if (this->size > 0) {
        this->nowChar = this->doc.substr(0, 1);
    }
}

Tree *Tree::load(std::string str) {
    this->prepare(str);
    while (this->parse());
    this->root->_[t_end] = this->cursor;
    return this;
}

bool Tree::parse() {
    std::string s;
    if ((s = this->copyUntilChar("<")) == "") {
        return this->readTag();
    }
    Node node(this);
    ++this->cursor;
    node._[t_text] = s;
    this->linkNodes(node, false);
    return true;
}

void Tree::dump(bool isShowAttr) {
    this->root->dump(isShowAttr);
}