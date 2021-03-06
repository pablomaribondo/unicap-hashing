#include <cstdlib>
#include <iostream>
#include "HashTable_EncadeamentoInterno.h"

using namespace std;

HashTable_EncadeamentoInterno::HashTable_EncadeamentoInterno() {
    htable = new Node*[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        htable[i] = NULL;
    }
}

HashTable_EncadeamentoInterno::~HashTable_EncadeamentoInterno() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        delete htable[i];
    }
    delete[] htable;
}

int HashTable_EncadeamentoInterno::hashFunction(String key) {
    return stringTransposition(key) % (TABLE_SIZE / 2);
}

int HashTable_EncadeamentoInterno::stringTransposition(String key) {
    int sum = 0, numericValue;
    for (int i = 0; i < key.length(); i++) {
        numericValue = int (key.at(i));
        sum += numericValue << i % 8;
    }
    return (abs(sum) % TABLE_SIZE) + 1;
}

void HashTable_EncadeamentoInterno::insert(String key, String value) {
    if (find(key, value) == -1) {
        if (!this->isFull()) {
            int hash_val = hashFunction(key);
            Node* newNode = new Node(key, value);
            if (htable[hash_val] == NULL) {
                htable[hash_val] = newNode;
            } else if (htable[hash_val]->getNext() == NULL) {
                for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++) {
                    if (htable[i] == NULL) {
                        htable[i] = newNode;
                        htable[hash_val]->setNext(newNode);
                        break;
                    }
                }
            } else {
                int position = find(htable[hash_val]->getNext()->getKey(), htable[hash_val]->getNext()->getValue());
                insert_position(key, value, position);
            }
        } else {
            cout << "The Hash Table is full!" << endl;
        }
    } else {
        //        cout << "This Element already exists!" << endl;
    }
}

void HashTable_EncadeamentoInterno::insert_position(String key, String value, int position) {
    Node* newNode = new Node(key, value);
    if (htable[position] == NULL) {
        htable[position] = newNode;
    } else if (htable[position]->getNext() == NULL) {
        for (int i = position; i < TABLE_SIZE; i++) {
            if (htable[i] == NULL) {
                htable[i] = newNode;
                htable[position]->setNext(newNode);
                break;
            }
        }
    } else {
        int newPosition = find(htable[position]->getNext()->getKey(), htable[position]->getNext()->getValue());
        insert_position(key, value, newPosition);
    }
}

void HashTable_EncadeamentoInterno::remove(String key, String value) {
    int hash_val = hashFunction(key);
    int position = find(key, value);
    if (position == -1) {
        cout << "This element does not exist!" << endl;
    } else {
        int position_previous = search_previous(key, value);
        if (position_previous == -1) {
            if (htable[position]->getNext() == NULL) {
                delete htable[position];
                htable[position] = NULL;
            } else {
                int position_next = find(htable[position]->getNext()->getKey(), htable[position]->getNext()->getValue());
                delete htable[position];
                htable[position] = new Node(htable[position]->getNext()->getKey(), htable[position]->getNext()->getValue(), htable[position]->getNext()->getNext());
                if (htable[position_next]->getNext() == NULL) {
                    delete htable[position_next];
                    htable[position_next] = NULL;
                } else {
                    htable[position]->setNext(htable[position]->getNext());
                    delete htable[position_next];
                    htable[position_next] = NULL;
                }
            }
        } else {
            if (htable[position]->getNext() == NULL) {
                htable[position_previous]->setNext(NULL);
                delete htable[position];
                htable[position] = NULL;
            } else {
                int position_next = find(htable[position]->getNext()->getKey(), htable[position]->getNext()->getValue());
                htable[position_previous]->setNext(htable[position_next]);
                delete htable[position];
                htable[position] = NULL;
            }
        }
    }
}

void HashTable_EncadeamentoInterno::remove_position(int position) {
    if (htable[position] != NULL) {
        String key = htable[position]->getKey();
        String value = htable[position]->getValue();
        this->remove(key, value);
    }
}

int HashTable_EncadeamentoInterno::find(String key, String value) {
    int position = -1;
    int hash_val = hashFunction(key);
    if (htable[hash_val] == NULL) {
        return position;
    } else if (htable[hash_val]->getKey() == key && htable[hash_val]->getValue() == value) {
        return hash_val;
    } else {
        Node* next = htable[hash_val]->getNext();
        while (next != NULL) {
            if (next->getKey() == key && next->getValue() == value) {
                for (int i = (TABLE_SIZE / 2); i < TABLE_SIZE; i++) {
                    if (htable[i] == NULL) {
                        continue;
                    } else if (htable[i]->getKey() == key && htable[i]->getValue() == value) {
                        position = i;
                        goto FOUND_SEARCH;
                    }
                }
            }
            next = next->getNext();
        }
FOUND_SEARCH:
        return position;
    }
}

int HashTable_EncadeamentoInterno::search_previous(String key, String value) {
    int position = -1;
    int hash_val = hashFunction(key);
    if (htable[hash_val] == NULL || htable[hash_val]->getNext() == NULL || (htable[hash_val]->getKey() == key && htable[hash_val]->getValue() == value)) {
        return position;
    } else {
        Node* next = htable[hash_val]->getNext();
        while (next != NULL) {
            if (next->getKey() == key && next->getValue() == value) {
                for (int i = (TABLE_SIZE / 2); i < TABLE_SIZE; i++) {
                    if (htable[i] == NULL || htable[i]->getNext() == NULL) {
                        continue;
                    } else if (htable[i]->getNext()->getKey() == key && htable[i]->getNext()->getValue() == value) {
                        position = i;
                        goto FOUND_SEARCH_POSITION;
                    }
                }
            }
            next = next->getNext();
        }
FOUND_SEARCH_POSITION:
        return position;
    }
}

void HashTable_EncadeamentoInterno::displayAll() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (htable[i] == NULL) {
            cout << "Empty" << endl;
            continue;
        }
        cout << "[Key: " << htable[i]->getKey() << " | Value: " << htable[i]->getValue() << "]";
        if (htable[i]->getNext() == NULL) {
            cout << endl;
            continue;
        } else {
            cout << " -> [Key: " << htable[i]->getNext()->getKey() << "]";
        }
        cout << endl;
    }
}

bool HashTable_EncadeamentoInterno::isFull() {
    bool flag = true;
    for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++) {
        if (htable[i] != NULL) {
            continue;
        } else {
            flag = false;
            break;
        }
    }
    return flag;
}