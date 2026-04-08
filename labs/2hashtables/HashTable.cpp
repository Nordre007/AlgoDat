#include "HashTable.h"
#include <functional> // for std::hash

using namespace std;

size_t HashTable::hashFunction(const string& key) {
    return hash<string>{}(key) % capacity;
}

void HashTable::rehash(int newCapacity) {
    vector<vector<pair<string, int>>> old = move(hashMap);

    capacity = newCapacity;
    hashMap.resize(capacity);
    numberOfElements = 0;

    for(auto& list : old) {
        for(const auto& [k, v] : list) {
            insert(k, v);
        }
    }
}

double HashTable::loadFactor(int numberOfElements, int capacity) {
    return (double) numberOfElements / capacity;
}


HashTable::HashTable(): capacity{1}, numberOfElements{0} {
    hashMap.resize(capacity);
}; 

const vector<vector<pair<string,int>>>& HashTable::getTable() const {
    return hashMap;
}
        
void HashTable::insert(const string& key, int value) {
    int i = hashFunction(key);
    
    for(auto& [k, v]: hashMap[i]) {
        if(k == key) {
            v = value;
            return;
        }
    }

    hashMap[i].emplace_back(key, value);
    numberOfElements++;
    
    if(loadFactor(numberOfElements, capacity) > 0.75) {
        rehash(capacity * 2);
    } 
}

bool HashTable::erase(const string& key) {
    int i = hashFunction(key);
    auto& internalList = hashMap[i];

    for(int i = 0; i < (int) internalList.size(); i++) {
        if(internalList[i].first == key) {
            internalList[i] = internalList.back();
            internalList.pop_back();
            numberOfElements--;
            return true;
        }
    }

    if(capacity > 1 && loadFactor(numberOfElements, capacity) < 0.25) {
        rehash(capacity / 2);
    }

    return false;
}

bool HashTable::isPresent(const string& key) {
    int i = hashFunction(key);
    const auto& internalList = hashMap[i];

    for(int i = 0; i < (int) internalList.size(); i++) {
        if(internalList[i].first == key) {
            return true;
        }
    }

    return false;
}

int HashTable::getValue(const string& key ) {
    int i = hashFunction(key);
    auto& internalList = hashMap[i];

    for(int i = 0; i < (int) internalList.size(); i++) {
        if(internalList[i].first == key) {
            return internalList[i].second;
        }
    }

    return 0;
}
