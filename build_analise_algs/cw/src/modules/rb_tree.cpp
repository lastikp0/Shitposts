#include "rb_tree.h"

#include <iostream>
#include <string>
#include <utility>

RBTree::RBTree():
    root_(nullptr)
{}

RBTree::~RBTree()
{
    clear(root_);
}

void RBTree::insert(int key, std::string data)
{
    Node* node = new Node(key, data);
    Node* parent = nullptr;
    Node* current = root_;

    while (current != nullptr) {
        parent = current;

        if (node->key_ < current->key_) {
            current = current->left_;
        } else {
            current = current->right_;
        }
    }

    node->parent_ = parent;
    if (parent == nullptr) {
        root_ = node;
    } else if (node->key_ < parent->key_) {
        parent->left_ = node;
    } else {
        parent->right_ = node;
    }
    
    fixInsert(node);
}

void RBTree::remove(int key)
{
    Node* node = root_;
    Node* x = nullptr;
    Node* y = nullptr;

    while (node != nullptr) {
        if (node->key_ == key) {
            break;
        }

        if (node->key_ > key) {
            node = node->left_;
        } else {
            node = node->right_;
        }
    }

    if (node == nullptr) {
        std::cout << "Key not found" << std::endl;
        return;
    }

    y = node;
    Color yOriginalColor = y->color_;
    if (node->left_ == nullptr) {
        x = node->right_;
        transplant(node, node->right_);
    } else if (node->right_ == nullptr) {
        x = node->left_;
        transplant(node, node->left_);
    } else {
        y = minKeyNode(node->right_);
        yOriginalColor = y->color_;
        x = y->right_;

        if (y->parent_ != node) {
            transplant(y, y->right_);
            y->right_ = node->right_;
            y->right_->parent_ = y;
        }

        transplant(node, y);
        y->left_ = node->left_;
        y->left_->parent_ = y;
        y->color_ = node->color_;
    }

    delete node;
    if (yOriginalColor == BLACK) {
        fixRemove(x);
    }
}

std::string RBTree::find(int key)
{
    Node* node = root_;

    while (node != nullptr) {
        if (node->key_ == key) {
            break;
        }

        if (node->key_ > key) {
            node = node->left_;
        } else {
            node = node->right_;
        }
    }

    if (node == nullptr) {
        std::cout << "Key not found" << std::endl;
        return "";
    } else {
        return node->data_;
    }
}

void RBTree::print()
{
    if (root_ == nullptr) {
        std::cout << "Tree is empty." << std::endl;
    } else {
        print(root_, "", 0);
        std::cout <<std::endl;
    }
}

void RBTree::getData(std::vector<Task>& res)
{
    res.clear();
    getData(root_, res);
}

void RBTree::clear()
{
    clear(root_);
    root_ = nullptr;
}

void RBTree::rotateL(Node* node)
{
    Node* child = node->right_;
    node->right_ = child->left_;

    if (node->right_ != nullptr) {
        node->right_->parent_ = node;
    }
        
    child->parent_ = node->parent_;
    if (node->parent_ == nullptr) {
        root_ = child;
    } else if (node == node->parent_->left_) {
        node->parent_->left_ = child;
    } else {
        node->parent_->right_ = child;
    }
        
    child->left_ = node;
    node->parent_ = child;
}

void RBTree::rotateR(Node* node)
{
    Node* child = node->left_;
    node->left_ = child->right_;

    if (node->left_ != nullptr) {
        node->left_->parent_ = node;
    }
            
    child->parent_ = node->parent_;
    if (node->parent_ == nullptr) {
        root_ = child;
    } else if (node == node->parent_->left_) {
        node->parent_->left_ = child;
    } else {
        node->parent_->right_ = child;
    }
        
    child->right_ = node;
    node->parent_ = child;
}

void RBTree::fixInsert(Node* node)
{
    Node* parent = nullptr;
    Node* grandparent = nullptr;

    while (node != root_ && node->color_ == RED && node->parent_->color_ == RED) {
        parent = node->parent_;
        grandparent = parent->parent_;

        if (parent == grandparent->left_) {
            Node* uncle = grandparent->right_;

            if (uncle != nullptr && uncle->color_ == RED) {
                grandparent->color_ = RED;
                parent->color_ = BLACK;
                uncle->color_ = BLACK;
                node = grandparent;
            } else {
                if (node == parent->right_) {
                    rotateL(parent);
                    node = parent;
                    parent = node->parent_;
                }

                rotateR(grandparent);
                std::swap(parent->color_, grandparent->color_);
                node = parent;
            }
        } else {
            Node* uncle = grandparent->left_;

            if (uncle != nullptr && uncle->color_ == RED) {
                grandparent->color_ = RED;
                parent->color_ = BLACK;
                uncle->color_ = BLACK;
                node = grandparent;
            } else {
                if (node == parent->left_) {
                    rotateR(parent);
                    node = parent;
                    parent = node->parent_;
                }

                rotateL(grandparent);
                std::swap(parent->color_, grandparent->color_);
                node = parent;
            }
        }
    }
    
    root_->color_ = BLACK;
}

void RBTree::fixRemove(Node* node)
{
    while (node != root_ && node->color_ == BLACK) {
        if (node == node->parent_->left_) {
            Node* sibling = node->parent_->right_;

            if (sibling->color_ == RED) {
                std::swap(sibling->color_, node->parent_->color_);
                rotateL(node->parent_);
                sibling = node->parent_->right_;
            }

            if ((sibling->left_ == nullptr || sibling->left_->color_ == BLACK)
                && (sibling->right_ == nullptr || sibling->right_->color_ == BLACK))
            {
                sibling->color_ = RED;
                node = node->parent_;
            } else {
                if (sibling->right_ == nullptr || sibling->right_->color_ == BLACK) {
                    std::swap(sibling->left_->color_, sibling->color_);
                    rotateR(sibling);
                    sibling = node->parent_->right_;
                }

                std::swap(sibling->color_, node->parent_->color_);

                if (sibling->right_ != nullptr) {
                    sibling->right_->color_ = BLACK;
                }
                
                rotateL(node->parent_);
                node = root_;
            }
        } else {
            Node* sibling = node->parent_->left_;

            if (sibling->color_ == RED) {
                std::swap(sibling->color_, node->parent_->color_);
                rotateR(node->parent_);
                sibling = node->parent_->left_;
            }

            if ((sibling->left_ == nullptr || sibling->left_->color_ == BLACK)
                && (sibling->right_ == nullptr || sibling->right_->color_ == BLACK))
            {
                sibling->color_ = RED;
                node = node->parent_;
            } else {
                if (sibling->left_ == nullptr || sibling->left_->color_ == BLACK) {
                    std::swap(sibling->right_->color_, sibling->color_);
                    rotateL(sibling);
                    sibling = node->parent_->left_;
                }

                std::swap(sibling->color_, node->parent_->color_);

                if (sibling->left_ != nullptr) {
                    sibling->left_->color_ = BLACK;
                }
                    
                rotateR(node->parent_);
                node = root_;
            }
        }
    }

    node->color_ = BLACK;
}

RBTree::Node* RBTree::minKeyNode(Node* node)
{
    Node* current = node;
    while (current->left_ != nullptr) {
        current = current->left_;
    }
    
    return current;
}

void RBTree::transplant(Node* u, Node* v)
{
    if (u->parent_ == nullptr) {
        root_ = v;
    } else if (u == u->parent_->left_) {
        u->parent_->left_ = v;
    } else {
        u->parent_->right_ = v;
    }
        
    if (v != nullptr) {
        v->parent_ = u->parent_;
    }
}

void RBTree::print(Node* node, std::string indent, int l_or_r)
{
    if (node != nullptr) {
        std::cout << indent;

        if (l_or_r == 1) {
            std::cout << "R----";
            indent += "   ";
        } else if (l_or_r == -1) {
            std::cout << "L----";
            indent += "|  ";
        } else {
            std::cout << "Root  ";
            indent += "    ";
        }

        std::string sColor = (node->color_ == RED) ? "RED" : "BLACK";
        std::cout << node->key_ << ": " << node->data_ << "(" << sColor << ")" << std::endl;

        print(node->left_, indent, -1);
        print(node->right_, indent, 1);
    }
}

void RBTree::getData(Node* node, std::vector<Task>& res)
{
    if (node == nullptr) {
        return;
    }

    std::vector<Task> left;
    getData(node->left_, left);
    std::vector<Task> right;
    getData(node->right_, right);

    res.insert(res.end(), left.begin(), left.end());
    res.insert(res.end(), {node->key_, node->data_});
    res.insert(res.end(), right.begin(), right.end());
}

void RBTree::clear(Node* node)
{
    if (node != nullptr) {
        clear(node->right_);
        clear(node->left_);
        delete node;
    }
}