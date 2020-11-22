#ifndef __FHEAP_H_
#define __FHEAP_H_

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>

template <typename T>
class PriorityQueue {
    public:
        virtual void insert(const T& item) = 0;
        virtual std::optional<T> extract_min() = 0;
        virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
    public:
        // constructors
        FibonacciNode()
            :key(std::nullopt), degree(0), child(nullptr), right(nullptr) {}
        FibonacciNode(const T& item)
            :key(item), degree(0), child(nullptr), right(nullptr) {}

        // destructor
        ~FibonacciNode() = default;

        T key;
        size_t degree;

        std::shared_ptr<FibonacciNode<T>> right;
        std::shared_ptr<FibonacciNode<T>> child;
        // NOTE: If you set left/parent pointer to shared_ptr, circular reference may cause!
        // So, left/parent pointer should be set to weak_ptr.
        std::weak_ptr<FibonacciNode<T>> left;
        std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
    public:
        // Default Constructor
        FibonacciHeap()
            : min_node(nullptr), size_(0) {}

        // Constructor with Value
        FibonacciHeap(const T& item)
            : min_node(nullptr), size_(0) { insert(item); }

        // Disable copy constructor.
        FibonacciHeap(const FibonacciHeap<T> &);

        // Constructor with initializer_list
        // ex) FibonacciHeap<int> fheap = {1, 2, 3, 4};
        FibonacciHeap(std::initializer_list<T> list): min_node(nullptr), size_(0) {
            for(const T& item : list) {
                insert(item);
            }
        }

        // Destructor
        ~FibonacciHeap();

        // Insert item into the heap.
        void insert(const T& item) override;

        // Return raw pointer of the min node.
        // If the heap is empty, return nullptr.
        FibonacciNode<T>* get_min_node() { return min_node.get(); }

        // Return minimum value of the min node.
        // If the heap is empty, return std::nullopt.
        std::optional<T> get_min() const;

        // 1. Return minimum value of the min node
        // 2. Remove the node which contains minimum value from the heap.
        // If the heap is empty, return std::nullopt;
        std::optional<T> extract_min() override;

        // Return true if the heap is empty, false if not.
        bool is_empty() const override { return !size_; }

        // Return the number of nodes the heap contains.
        size_t size() const { return size_; }


    private:
        // Points to the node which has minimum value.
        std::shared_ptr<FibonacciNode<T>> min_node;

        // Value that represents how many nodes the heap contains.
        size_t size_;

        void insert(std::shared_ptr<FibonacciNode<T>>& node);

        // After extract, clean up the heap.
        void consolidate();

        // Combine two nodes.
        void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
};

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    
    // TODO
}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
    if(is_empty())
        return std::nullopt;
    else
        return min_node->key;
    // TODO
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    
    std::shared_ptr<FibonacciNode<T>> new_node = std::make_shared<FibonacciNode<T>>(item);
    if(is_empty()){
        min_node = new_node;
        min_node->left = min_node;
        min_node->right = min_node;
        size_++;
        return;
    }
    else{
        new_node->left = min_node->left;
        new_node->right = min_node;
        min_node->left.lock()->right = new_node;
        min_node->left = new_node;
        size_++;
        if (min_node->key<item){
            return;
        }
        else{
            min_node = new_node;
            return;
        }
    }
    // TODO
}

template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {
    node->left.lock()->right = node->child;
    node->child->left.lock()->right = node->right;
    node = node->right;
    return;
    // TODO
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    auto temp = min_node;
    if(is_empty()) return std::nullopt;
    if(min_node->child != nullptr){ // min_node has child
        insert(min_node);
        T result = temp->key;
        temp.reset();
        size_ --;
        consolidate();
        return result;
    }
    else{
        min_node->left.lock()->right = min_node->right;
        min_node->right->left = min_node->left;
        min_node = min_node->right;
        T result = temp->key;
        temp.reset();
        size_ --;
        consolidate();
        return result;
    }
    // TODO
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    std::shared_ptr<FibonacciNode<T>> *heap = new std::shared_ptr<FibonacciNode<T>>[size_];
    auto temp = min_node;
    size_t n = temp->degree;
    do {
        n = temp->degree;
        if(min_node->key>temp->key) min_node = temp;
        if(heap[n]!=nullptr){
            if(heap[n]==temp) continue;
            while(heap[n]!=nullptr){
                if(heap[n]->key<temp->key){
                    merge(heap[n], temp);
                    temp = heap[n];
                }
                else {
                    merge(temp,heap[n]);
                    heap[n] = temp;
                }
                std::cout<<heap[n]->key;
                heap[n] = temp;
            }
        }
        else {heap[n] = temp;}
        temp = temp->right;
    } while (temp->right!=min_node);
    
    delete[] heap;
    return;
    // TODO
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    y->parent = x;
    y->left.lock()->right = y->right;
    y->right->left = y->left;
    // linking for extracted y's position
    if(x->child == nullptr)
    {
        x->child = y;
        x->child->right = y;
        x->child->left = y;
        y->left = y;
        y->right=y;
        return;
    }
    y->right = x->child;
    y->left  = x->child->left;
    x->child->left.lock()->right = y;
    x->child->left = y;
    x->child = y;
    x->degree = x->degree + y->degree + 1;
    return;
    // TODO
}

#endif // __FHEAP_H_

int main(){
    
//    FibonacciHeap<int> heap;
//    heap.insert(5);
//    heap.insert(3);
//    heap.insert(7);
//    heap.insert(2);
//    std::cout<<heap.extract_min().has_value()<<std::endl;
    FibonacciHeap<int> heap = {1, 9, 5, 3, 7};
    std::cout<<heap.extract_min().has_value()<<std::endl;
    std::cout<<heap.extract_min().has_value()<<std::endl;
    std::cout<<heap.extract_min().has_value()<<std::endl;
    std::cout<<heap.extract_min().has_value()<<std::endl;
    std::cout<<heap.extract_min().has_value()<<std::endl;
    
    
    
}
