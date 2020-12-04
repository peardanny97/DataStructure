#ifndef _DEQUE_H
#define _DEQUE_H

#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
#include <iostream>
#include <memory>
#include <cassert>

/* NOTE: Deque, ArrayDeque, ListDeque Declaration modification is not allowed.
 * Fill in the TODO sections in the following code. */
template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    /* NOTE: We won't implement push functions that take rvalue references. */
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;

    /* NOTE: Unlike STL implementations which have separate `front` and
       pop_front` functions, we have one unified method for removing an elem. */
    virtual std::optional<T> remove_front() = 0;
    virtual std::optional<T> remove_back() = 0;

    virtual bool empty() = 0;
    virtual size_t size() = 0;

    virtual T& operator[](size_t) = 0;
};

template <typename T>
class ArrayDeque : public Deque<T> {
public:
    ArrayDeque();
    ~ArrayDeque() = default;

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;
    size_t capacity();

    T& operator[](size_t) override;

private:
    std::unique_ptr<T[]> arr;
    size_t front;
    size_t back;
    size_t size_;
    size_t capacity_;

    void resize();
};

template <typename T>
ArrayDeque<T>::ArrayDeque() :
    front{63 /* You can change this */},
    back{0 /* You can change this */},
    size_{0}, capacity_{64} {
    arr = std::make_unique<T[]>(capacity_);
}

template <typename T>
void ArrayDeque<T>::push_front(const T& item) {
    
	size_++;
	if(size_>capacity_){
		resize();	
	}
	arr[front] = item;
	if(front==0) front=capacity_-1;
	else front --;
	
	return;
	// TODO
}

template <typename T>
void ArrayDeque<T>::push_back(const T& item) {
    
    size_++;
	if(size_>capacity_){
		resize();
	}
	arr[back] = item;
	if(back == capacity_-1) back = 0;
	else back++;
    
	return;
    // TODO
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_front() {
	if(size_==0) return std::nullopt;
    size_--;
	if(front == capacity_-1){
		 front = 0; return arr[front];}
    else front ++; return arr[front];
	// TODO
    return std::nullopt;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_back() {
	if(size_==0){return std::nullopt;}
	size_--;
    if(back == 0) back = capacity_-1;
    else back--;                                                                              
    return arr[back];// TODO
    return std::nullopt;
}

template <typename T>
void ArrayDeque<T>::resize() {
	std::unique_ptr<T[]> new_arr;  
	new_arr = std::make_unique<T[]>(2*capacity_);
	
	for(int b=0;b<back;b++){
		new_arr[b] = arr[b];
	}
	for(int f=1;f<=capacity_-front;f++){
		new_arr[2*capacity_-f] = arr[capacity_-f];
	} 
	front = capacity_+front;
	capacity_=2*capacity_;
	arr = std::move(new_arr);
	new_arr.release();
	 // TODO
}

template <typename T>
bool ArrayDeque<T>::empty() {
   if(size_==0)	  return true; // TODO
    return false;
}

template <typename T>
size_t ArrayDeque<T>::size() {
    return size_; // TODO
    return 0;
}

template <typename T>
size_t ArrayDeque<T>::capacity() {
    return capacity_;// TODO
    return 0;
}

template <typename T>
T& ArrayDeque<T>::operator[](size_t idx) {
	
	if(front+idx+1<capacity_) return arr[front+idx+1];
	else return arr[front+idx-capacity_+1];// TODO
    return *new T{};
}

template<typename T>
struct ListNode {
    std::optional<T> value;
    ListNode* prev;
    ListNode* next;

    ListNode() : value(std::nullopt), prev(this), next(this) {}
    ListNode(const T& t) : value(t), prev(this), next(this) {}

    ListNode(const ListNode&) = delete;
};

template<typename T>
class ListDeque : public Deque<T> {
public:
    ListDeque();
    ~ListDeque();

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;

    T& operator[](size_t) override;

    size_t size_ = 0;
    ListNode<T>* sentinel = nullptr;
};

template<typename T>
ListDeque<T>::ListDeque() : sentinel(new ListNode<T>{}), size_(0) {}

template<typename T>
void ListDeque<T>::push_front(const T& t) {
	ListNode<T>* new_node = new ListNode(t);

	new_node -> next = sentinel->next;
	new_node -> prev = sentinel;
	sentinel -> next -> prev = new_node;
	sentinel -> next = new_node;
	size_++;
	return;
	
	// TODO
}

template<typename T>
void ListDeque<T>::push_back(const T& t) {
	 ListNode<T>* new_node = new ListNode(t);
	
	new_node -> prev = sentinel->prev;
	new_node -> next = sentinel;
	sentinel -> prev -> next = new_node;
	sentinel -> prev = new_node;
	size_++;
	return;
	
	// TODO
}

template<typename T>
std::optional<T> ListDeque<T>::remove_front() {
    if (empty()) return std::nullopt;
	else {
		ListNode<T>* front = sentinel->next;
		sentinel -> next = front ->next;
		
		sentinel -> next -> prev = sentinel;
		
		T result;
		result = front -> value.value(); 
		delete front;
		size_--;
		return result;
	}
	// TODO
    return std::nullopt;
}

template<typename T>
std::optional<T> ListDeque<T>::remove_back() {
    if (empty()) return std::nullopt;
	else {
		ListNode<T>* back = sentinel->prev;
		sentinel -> prev = back -> prev;
		
		sentinel -> prev -> next = sentinel;
		
		T result;
		result = back -> value.value();
		delete back;
		size_--;
		return result;
	}
	// TODO
    return std::nullopt;
}

template<typename T>
bool ListDeque<T>::empty() {
    if(size_==0) return true;// TODO
    return false;
}

template<typename T>
size_t ListDeque<T>::size() {
    return size_;// TODO
    return 0;
}

template<typename T>
T& ListDeque<T>::operator[](size_t idx) {
    size_t i = 0;
	ListNode<T>* temp = sentinel->next;
	while(i<idx){
		temp = temp-> next;
		i++;
	}
	return temp->value.value();	
	// TODO
    return *new T{};
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListNode<T>& n) {
    if (n.value)
        os << n.value.value();

    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListDeque<T>& l) {
    auto np = l.sentinel->next;
    while (np != l.sentinel) {
        os << *np << ' ';
        np = np->next;
    }

    return os;
}

template<typename T>
ListDeque<T>::~ListDeque() {

	size_t i = size_;
	
	while(i>0){
		ListNode<T>* temp = sentinel -> next;
		sentinel -> next = temp -> next;
		sentinel -> next -> prev =sentinel;
		delete temp;
		i--;
	}
	
	delete sentinel;// TODO
}

#endif // _DEQUE_H
