#include "hash_table.hpp"
#include <iostream>
int main()
{
    HashTable<int> h1(7);
    h1.insert(3, -5);
    h1.insert(10, -10);
    h1.insert(17, -15);
    int* ref1 = h1.get(3);
    int* ref2 = h1.get(17);
    HashTable<int> h2 = std::move(h1);
    std::cout << h1.numElements() << '\n';
    std::cout << h2.numElements() << '\n';
    std::cout << (ref1 == h2.get(3)) << '\n';
    std::cout << (ref2 == h2.get(17)) << '\n';
}