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
        // TODO
        return (pos+step);
    }
};

template <typename K, typename V, typename F>
class QuadProbeHashTable: public HashTable<K, V, F> {
private:
    virtual unsigned long get_next_pos(unsigned long pos, unsigned long step) {
        // TODO
        return (pos+0.5*step+0.5*step*step);
    }
};

template <typename K, typename V, typename F>
HashTable<K, V, F>::HashTable(): table(), hash_func(),
                                 size(0), table_size(INITIAL_TABLE_SIZE) {
    table = new HashSlot<K, V>[table_size];
}

template <typename K, typename V, typename F>
HashTable<K, V, F>::~HashTable() {
    // TODO
    delete []table;
}

template <typename K, typename V, typename F>
void HashTable<K, V, F>::enlarge_table() {
    // TODO
    //크기 2배해서 새로 만든다음 옮겨줌.
    table_size=table_size*2;
    HashSlot<K, V> *newTable;
    newTable=new HashSlot<K,V>[table_size];//크기 2배인 table 생성
    
    
    for(int i=0;i<table_size/2;i++){
        //table[i]가 empty거나 removed면 다음으로 넘어감.
        if(table[i].is_empty()==true||table[i].is_removed()==true){
            //아무것도 안함
        }
        else{
            //newTable에 put 해줘야함.
            auto Index=get_pos(table[i].get_key());
            auto ArrayIndex=get_pos(table[i].get_key());
            int step=0;
            //새로만든거 비어있으면 고대로 넣고 뭔가 들어있으면 다음꺼에 넣기
            while(newTable[Index].is_empty()!=true){//뭔가 들어있으면 다음으로 >넘어가기
                    step+=1;
                    Index=get_next_pos(ArrayIndex,step)%table_size;
            }
            newTable[Index].set_key_value(table[i].get_key(),table[i].get_value());
        }
    }
    delete []table;
    table=newTable;
   
}

template <typename K, typename V, typename F>
unsigned long HashTable<K, V, F>::get_pos(const K key) {
    // TODO만짐
    size_t M=get_table_size();
    return hash_func(key)%M;
    //return 0;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::get(const K &key, V &value) {
    // TODO
    //일단 주어진 key를 hash함수에 넣음.
    unsigned long ArrayIndex=get_pos(key);
    unsigned long newIndex=get_pos(key);
    int probe=0;
    int step=0;
    //키가 같고 삭제도 안됐을때 스탑.
    while(table[newIndex].get_key()!=key||table[newIndex].is_removed()==true)
    {
        step+=1;
        probe+=1;
        newIndex=get_next_pos(ArrayIndex,step)%table_size;
        //한바퀴 돌면 return -1
        //empty 여도 return -1
        if(newIndex==ArrayIndex){
            return -1;
        }
        
        else if(table[newIndex].is_empty()==true)
        {
            return -1;
        }
    }
        value=table[newIndex].get_value();
        return probe;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::put(const K &key, const V &value) {
    // TODO
    unsigned long ArrayIndex=get_pos(key);
    unsigned long newIndex=get_pos(key);
    int step=0;
    int probe=0;
    //일단 같은거 있는지 확인.
    //key가 같고 삭제되지도않았고 empty도 아닐때
    for(int i=0;i<get_table_size();i++){
        if(key==table[i].get_key()&&table[i].is_removed()!=true){
            if(table[i].is_empty()!=true){
            return -1;
            }
        }
    }
    // removed 거나 empty 나오면 put.
    while(table[newIndex].is_removed()!=true&&table[newIndex].is_empty()!=true){
        step+=1;
        probe+=1;
        newIndex=get_next_pos(ArrayIndex,step)%table_size;
    }
    table[newIndex].set_key_value(key,value);
    size+=1;
    if(get_load_factor()>=0.5){
        enlarge_table();
    }
    return probe;
}

template <typename K, typename V, typename F>
int HashTable<K, V, F>::remove(const K &key) {
    // TODOOK
    unsigned long ArrayIndex=get_pos(key);
    unsigned long newIndex=get_pos(key);
    int probe=0;
    int step=0;
    //removed아니고 key같으면 삭제. 다음꺼로 가다가 한바퀴 돌면 fail.
    while(table[newIndex].is_removed()==true||table[newIndex].get_key()!=key){
        
        step+=1;
        probe+=1;
        newIndex=get_next_pos(ArrayIndex,step)%table_size;
        if(newIndex==ArrayIndex){
        return -1;
        }
    }
    if(table[newIndex].is_empty()==true){
        return -1;
    }
    table[newIndex].set_removed();
    size=size-1;
    return probe;
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
