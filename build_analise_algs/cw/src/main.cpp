#include "rb_tree.h"

#include <iostream>

int main()
{
    RBTree rb_tree;

    rb_tree.insert(7, "text");
    rb_tree.insert(3, "sas");
    rb_tree.insert(18, "hffe");
    rb_tree.insert(10, "task");
    rb_tree.insert(20, "kefir");
    rb_tree.insert(8, "loot");
    rb_tree.insert(11, "lol");
    rb_tree.insert(26, "lmao");
    rb_tree.insert(23, "sleep");
    rb_tree.insert(21, "tree");
    rb_tree.insert(2, "hahah");
    rb_tree.insert(6, "q");
    rb_tree.print();

    rb_tree.remove(18);
    rb_tree.remove(11);
    rb_tree.remove(3);
    rb_tree.print();

    std::cout << "Task 7: " << rb_tree.find(7) << std::endl;

    std::vector<Task> vec;
    rb_tree.getData(vec);
    for (auto e: vec) {
        std::cout << e.key_ << " " << e.data_ << std::endl;
    }

    return 0;
}