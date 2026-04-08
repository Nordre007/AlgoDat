#ifndef OPENLINEARHASHTABLE_H
#define OPENLINEARHASHTABLE_H

#include <vector>
#include <string>
#include <utility>

using namespace std; 

enum class State {
    EMPTY,
    OCCUPIED,
    DELETED
};

struct Entry {
    string key;
    int value;
    State state;

    Entry(): key{""}, value{0}, state{State::EMPTY} {};
    
    Entry(const string& k, int v, State s)
    : key(k), value(v), state(s) {}
};

class OpenLinearHashTable {
    private:
        vector<Entry> hashMap;
        int capacity;
        int numberOfelements;
        int numberOfdeleted;

        size_t hashFunction(const std::string& key);
        void rehash(int newCapacity);
        double loadFactor(int numberOfElements, int capacity);

    public:
        OpenLinearHashTable();

        void insert(const string& key, int value);
        bool erase(const string& key);
        int getValue(const string& key);
        bool isPresent(const string& key);
        const vector<Entry>& getTable() const;

};



#endif
