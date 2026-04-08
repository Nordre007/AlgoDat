#include <bits/stdc++.h>
#include "HashTable.h"
#include <boost/algorithm/string.hpp>



using namespace std;

// read words from input, one word per line
// then use a dictionary to count which word is most frequent
// but sometimes try to remove the word
// and then print the most frequent word and if there are multiple
// most frequent take the first one in alphabetical order


int main () {

    int i{0};
    HashTable d;

    string word;
    while(getline(cin, word)) {

        boost::algorithm::trim(word);

        bool is_present {d.isPresent(word)};
        bool remove_it { i % 16 == 0};


        if(is_present) {
            if(remove_it) {
                d.erase(word);
            } else {
                d.insert(word, d.getValue(word) + 1);
            }

        } else if (!remove_it) {
            d.insert(word, 1);
        }

        i += 1;
    }

    string bestWord;
    int bestCount = -1;
    for(const auto& internalList : d.getTable()) {
        for (const auto& [word, count]: internalList) {
            if(count > bestCount) {
                bestCount = count;
                bestWord = word;
            }
        }
    }
    for(const auto& internalList : d.getTable()) {
        for (const auto& [word, count]: internalList) {
            if(d.getValue(word) == bestCount && word < bestWord ) {
                bestWord = word;
            }
        }
    }    
    cout << bestWord << " " << bestCount << "\n";


    return 0;
}