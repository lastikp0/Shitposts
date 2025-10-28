#include "rb_tree.h"

#include <iostream>
#include <cassert>
#include <algorithm>

int main() {
    RBTree rb_tree;

    rb_tree.insert(7, "a");
    rb_tree.insert(3, "b");
    rb_tree.insert(18, "c");
    rb_tree.insert(10, "d");
    rb_tree.insert(22, "e");
    rb_tree.insert(8, "f");
    rb_tree.insert(11, "g");
    rb_tree.insert(26, "h");
    rb_tree.insert(2, "i");
    rb_tree.insert(6, "j");

    std::vector<Task> vec;
    rb_tree.getData(vec);
    std::vector<std::string> res = {"i", "b", "j", "a", "f", "d", "g", "c", "e", "h"};
    
    bool flag = true;
    for (int i = 0; i < vec.size(); i++) {
        if (i > res.size()) {
            flag = false;
            break;
        }

        if (vec[i].data_ != res[i]) {
            flag = false;
            break;
        }
    }
    assert(flag);

    rb_tree.remove(18);
    rb_tree.remove(11);
    rb_tree.remove(3);

    rb_tree.getData(vec);
    res = {"i", "j", "a", "f", "d", "e", "h"};
    
    flag = true;
    for (int i = 0; i < vec.size(); i++) {
        if (i > res.size()) {
            flag = false;
            break;
        }

        if (vec[i].data_ != res[i]) {
            flag = false;
            break;
        }
    }
    assert(flag);

    std::string data = rb_tree.find(8);
    assert(data == "f");

    std::cout << "All tests passed" << std::endl;

    return 0;
}