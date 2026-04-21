#include "OpenLinearHashTable.h"
#include <functional> // for std::hash

using namespace std;

size_t OpenLinearHashTable::hashFunction(const string& key) {
    return hash<string>{}(key) & (capacity -1);
}

double OpenLinearHashTable::loadFactor(int numberOfElements, int capacity) {
    return (double) numberOfElements / capacity;
}

void OpenLinearHashTable::rehash(int newCapacity) {
    vector<Entry> old = move(hashMap);

    capacity = newCapacity;
    numberOfelements = 0;
    numberOfdeleted = 0;
    hashMap.clear();
    hashMap.resize(newCapacity);

    for(auto& entry : old) {
        if(entry.state == State::OCCUPIED) {
            insert(entry.key, entry.value);
        }
    }

}

OpenLinearHashTable::OpenLinearHashTable(): capacity{8}, numberOfelements{0}, numberOfdeleted{0} {
    hashMap.resize(capacity);
}

void OpenLinearHashTable::insert(const string& key, int value) {

    if (numberOfdeleted > numberOfelements) {
        rehash(capacity);
    }   

    if (loadFactor(numberOfelements + numberOfdeleted + 1, capacity) > 0.5) {
        rehash(capacity * 2);
    }

    int i = hashFunction(key);
    int firstDeleted = -1;


    while(true) {
        if(hashMap[i].state == State::EMPTY) {
            if (firstDeleted != -1) i = firstDeleted;

            hashMap[i] = {key, value, State::OCCUPIED};
            numberOfelements++;
            return;
        } else if(hashMap[i].state == State::DELETED) {
            if (firstDeleted == -1) firstDeleted = i;
        }

        if(hashMap[i].state == State::OCCUPIED && 
            hashMap[i].key == key) {
                hashMap[i].value = value;
                return;
        }

        i = (i + 1) % capacity;
    }

}

bool OpenLinearHashTable::erase(const string& key) {
    int i = hashFunction(key);

    while(hashMap[i].state != State::EMPTY) {
        if(hashMap[i].key == key && 
            hashMap[i].state == State::OCCUPIED) {
           
            hashMap[i].state = State::DELETED;
            numberOfelements--;
            numberOfdeleted++; 

            /*if(capacity > 8 && loadFactor(numberOfelements, capacity) < 0.0) {
                rehash(capacity / 2);
            }*/

            return true;
        }
        i = (i + 1) % capacity;
    }
    return false;
}

int OpenLinearHashTable::getValue(const string& key) {
    int i = hashFunction(key);
    int start = i;

    while(hashMap[i].state != State::EMPTY) {
        if(hashMap[i].state != State::DELETED 
            && hashMap[i].key == key ) {
                return hashMap[i].value;
            }
        i = (i + 1) % capacity;

        if(i == start) {
            break;
        }
    }

    return 0;
}

bool OpenLinearHashTable::isPresent(const string& key) {
    int i = hashFunction(key);
    int start = i;

    while (hashMap[i].state != State::EMPTY) {
        if (hashMap[i].state == State::OCCUPIED &&
            hashMap[i].key == key) {
            return true;
        }

        i = (i + 1) % capacity;

        if (i == start) {
            break; 
        }
    }

    return false;
}

const vector<Entry>& OpenLinearHashTable::getTable() const {
    return hashMap;
}
