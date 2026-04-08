#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include <utility>

class HashTable {
private:
    std::vector<std::vector<std::pair<std::string, int>>> hashMap;
    int capacity;
    int numberOfElements;

    size_t hashFunction(const std::string& key);
    void rehash(int newCapacity);
    double loadFactor(int numberOfElements, int capacity);

public:
    HashTable();

    const std::vector<std::vector<std::pair<std::string,int>>>& getTable() const;

    void insert(const std::string& key, int value);
    bool erase(const std::string& key);
    bool isPresent(const std::string& key);
    int getValue(const std::string& key);
};

#endif
