#include <iostream>
#include <string>
#include <vector>

struct Task
{
    int key_;
    std::string data_;
};

enum Color
{
    RED,
    BLACK
};

class RBTree
{
public:
    RBTree();
    ~RBTree();

    void insert(int key, std::string data);
    void remove(int key);

    std::string find(int key);

    void print();

    void getData(std::vector<Task>& res);

    void clear();

private:
    struct Node
    {
        std::string data_;

        int key_;
        Color color_;

        Node* parent_;
        Node* left_;
        Node* right_;

        Node(int key, std::string data);
    };

    Node* root_;

    void rotateL(Node* node);
    void rotateR(Node* node);

    void fixInsert(Node* node);
    void fixRemove(Node* node);

    Node* minKeyNode(Node* node);

    void transplant(Node* u, Node* v);

    void print(Node* node, std::string indent, int l_or_r);

    void getData(Node* node, std::vector<Task>& res);

    void clear(Node* node);
};