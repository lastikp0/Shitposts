#include "rb_tree.h"

RBTree::Node::Node(int key, std::string data):
    key_(key),
    data_(data),
    color_(RED),
    parent_(nullptr),
    left_(nullptr),
    right_(nullptr)
{}