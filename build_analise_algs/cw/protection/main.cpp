#include "rb_tree.h"

#include <iostream>
#include <fstream>
#include <string>

const std::string path = "./save.txt";

int command()
{
    int n;
    std::cout << "Input command(0 - exit, 1 - input, 2 - output, 3 - clear, 4 - save, 5 - load): ";
    std::cin >> n;
    return n;
}

void input(std::vector<Task>& vec)
{
    vec.clear();

    int n;
    std::cout << "Input tasks count: ";
    std::cin >> n;

    int key;
    std::string data;
    for (int i = 0; i < n; i++) {
        std::cout << "Input task(<priority> <text>): ";
        std::cin >> key >> data;
        vec.push_back({key, data});
    }
}

void save(RBTree& rb_tree)
{
    std::ofstream out;
    out.open(path);

    std::vector<Task> vec;

    if (out.is_open()) {
        rb_tree.getData(vec);
        out << (int)vec.size() << "\n";
        for (auto e: vec) {
            out << e.key_ << " " << e.data_ << "\n";
        }
    }
    out.close(); 
}

void load(RBTree& rb_tree)
{
    int n;
    rb_tree.clear();

    std::ifstream in(path);
    if (in.is_open()) {
        in >> n;

        int key;
        std::string data;
        for (int i = 0; i < n; i++) {
            in >> key >> data;
            rb_tree.insert(key, data);
        }
    }
    in.close();
}

int main()
{
    int n = command();

    RBTree rb_tree;
    std::vector<Task> vec;

    while (n) {
        if (n == 1) {
            input(vec);
            for (auto e: vec) {
                rb_tree.insert(e.key_, e.data_);
            }
        } else if (n == 2) {
            rb_tree.getData(vec);
            for (auto e: vec) {
                std::cout << e.key_ << " " << e.data_ << std::endl;
            }
        } else if (n == 3) {
            rb_tree.clear();
        } else if (n == 4) {
            save(rb_tree);
        } else if (n == 5) {
            load(rb_tree);
        }

        n = command();
    }

    return 0;
}