#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>

#define INITIAL_TABLE_SIZE 64

#include "hash_slot.hpp"

/* Fill in the TODO sections in the following code. */

template <typename K, typename V, typename F>
class HashTable {
public:
    HashTable();
    ~HashTable();
    int get(const K &key, V &value);
    int put(const K &key, const V &value);
    int remove(const K &key);
    size_t get_table_size();
    size_t get_size();
    double get_load_factor();

protected:
    size_t table_size;
    
private:
    HashTable(const HashTable & other);
    const HashTable & operator=(const HashTable & other);
    F hash_func;
    size_t size;
    HashSlot<K, V> *table;

    // Should be overriden by the derived class
    virtual unsigned long get_next_pos(unsigned long pos,
                                       unsigned long step) = 0;
    unsigned long get_pos(const K key);
    void enlarge_table();
};

template <typename K, typename V, typename F>
class LinearProbeHashTable: public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        return pos+step;
        // TODO
        return 0;
    }
};

template <typename K, typename V, typename F>
class QuadProbeHashTable: public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        return pos + 0.5*step + 0.5*step*step;
        // TODO
        return 0;
    }
};

template <typename K, typename V, typename F>
HashTable<K, V, F>::HashTable(): table(), hash_func(),
                                 size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V, typename F>
HashTable<K, V, F>::~HashTable() {
    delete [] table;
    // TODO
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::enlarge_table() {
    HashSlot<K, V> *new_table;
    size_t M = table_size;
    table_size = 2*table_size;
    new_table = new HashSlot<K, V>[table_size];
    for (int i=0;i<M;i++){
        if(!table[i].is_empty()){
            K key = table[i].get_key();
            V value = table[i].get_value();
            int probe = 0;
            unsigned long initial_pos = get_pos(key);
            unsigned long pos = initial_pos;
            while(1){
                if(new_table[pos].is_empty())
                {
                    new_table[pos].set_key_value(key,value);
                    break;
                }
                probe++;
                pos = get_next_pos(initial_pos, probe)%table_size;
            }
        }
            
    }
    auto temp = table;
    table = new_table;
    delete[] temp;
    // TODO
}

template <typename K, typename V, typename F>
unsigned long HashTable<K, V, F>::get_pos(const K key) {
    return hash_func(key)%table_size;
    // TODO
    return 0;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::get(const K &key, V &value) {
    int probe = 0;
    unsigned long initial_pos = get_pos(key);
    unsigned long pos = initial_pos;
    while(1){
        if(table[pos].is_empty()&&!table[pos].is_removed()){
            return -1;
        }
        if(!table[pos].is_empty()&&table[pos].get_key() == key){
            value = table[pos].get_value();
            return probe;
        }
        probe++;
        if(probe>=table_size) return -1;
        pos = get_next_pos(initial_pos, probe)%table_size;
        
    }
    // TODO
    return 0;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::put(const K &key, const V &value) {
    int probe = 0;
    unsigned long initial_pos = get_pos(key);
    unsigned long pos = initial_pos;
    while(1){
        if(!table[pos].is_empty()&&table[pos].get_key() == key){
            return -1;
        }
        else{
            if(table[pos].is_empty()){
                table[pos].set_key_value(key,value);
                size++;
                if(get_load_factor()>0.5){
                    enlarge_table();
                }
                return probe;
            }
            probe++;
            pos = get_next_pos(initial_pos, probe)%table_size;
            
        }
    }
    // TODO
    return 0;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::remove(const K &key) {
    int probe = 0;
    unsigned long initial_pos = get_pos(key);
    unsigned long pos = initial_pos;
    while(1){
        if(table[pos].is_empty()&&!table[pos].is_removed()){
            return -1;
        }
        if(!table[pos].is_empty()&&table[pos].get_key() == key){
            table[pos].set_empty();
            table[pos].set_removed();
            size --;
            return probe;
        }
        probe++;
        if(probe>=table_size) return -1;
        pos = get_next_pos(initial_pos, probe)%table_size;
            
    }
    // TODO
    return 0;
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_table_size() {
    return table_size;
}

template <typename K, typename V, typename F>
size_t HashTable<K, V, F>::get_size() {
    return size;
}

template <typename K, typename V, typename F>
double HashTable<K, V, F>::get_load_factor() {
    return (double)size/table_size;
}
