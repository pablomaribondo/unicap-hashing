#ifndef HASHTABLE_HASHINGDUPLO_H
#define HASHTABLE_HASHINGDUPLO_H

#include "Node.h"

class HashTable_HashingDuplo {
private:
    Node** htable;
    const int TABLE_SIZE = 12000;
    int hashFunction(String key);
    int doubleHashFunction(String key);
    int reHashFunction(int index, String key);
    int stringTransposition(String key);
    bool isFull();
public:
    HashTable_HashingDuplo();
    ~HashTable_HashingDuplo();
    void insert(String key, String value);
    void remove(String key, String value);
    int find(String key, String value);
    void displayAll();
};

#endif /* HASHTABLE_HASHINGDUPLO_H */

