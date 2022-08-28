#include "priority_queue.hpp"
#include <iostream>
int main()
{
    PriorityQueue<std::string> p(20);
    p.insert(15, "window");
    p.insert(10, "door");
    p.insert(20, "house");
    p.insert(22, "bed");
    p.insert(7, "sleep");
    p.insert(8, "table");
    const unsigned* ref1 = p.getMinKey();
    const std::string* ref2 = p.getMinValue();
    std::cout << p.numElements() << '\n';
    PriorityQueue<std::string> p2 = std::move(p);
    std::cout << p2.numElements() << '\n';
    std::cout << (ref1 == p2.getMinKey()) << '\n';
    std::cout << (ref2 == p2.getMinValue()) << '\n';
}