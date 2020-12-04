#ifndef __FHEAP_H_
#define __FHEAP_H_
#include <random>
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
    while(!is_empty()){
        extract_min();
    }
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
        if (min_node->key<=item){
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
    if(node->right == node){
        node->right = nullptr;
        node = std::move(node->child);
        return;
    }
    node->left.lock()->right = node->child;
    node->child->left.lock()->right = node->right;
    node->right->left = node->child->left;
    node->child->left = node->left;
    node = std::move(node->right);
    // TODO
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    if(is_empty()) return std::nullopt;
    if(min_node->child != nullptr){ // min_node has child
        T result = min_node->key;
        insert(min_node);
        size_ --;
        consolidate();
        return result;
    }
    else{
        T result = min_node->key;
        min_node->left.lock()->right = min_node->right;
        min_node->right->left = min_node->left;
        min_node = std::move(min_node->right);
        size_ --;
        if(is_empty()) {
            min_node->right = nullptr;
            min_node.reset();
            
        }
        consolidate();
        return result;
    }
    // TODO
    return std::nullopt;
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    if(is_empty()) return;
    if(min_node->right==min_node) return;
    /*if(min_node->right->right==min_node){
        if(min_node->degree==min_node->right->degree){
            if(min_node->key<min_node->right->key){
                merge(min_node,min_node->right);
                return;
            }
            else{
                merge(min_node->right,min_node);
                min_node = min_node->parent.lock();
                return;
            }
        }
        else return;
    }*/ //없애도 될까...?
    std::shared_ptr<FibonacciNode<T>> *heap = new std::shared_ptr<FibonacciNode<T>>[size_];
    //auto it = min_node->right;
    auto temp = min_node;
    auto ckpt = min_node->left;
    size_t n = temp->degree;
    bool check = false;
    while(!check) {
        //std::cout<<"process start min key is:"<<min_node->key<<std::endl;
        //std::cout<<"and temp key is:"<<temp->key<<std::endl;
        //std::cout<<"and it key is:"<<it->key<<std::endl;
        /*if(temp->right->right == temp) {
            if(temp->right->key<temp->key) min_node = temp->right;
            else min_node = temp;
            break;
            
        }*/
        if(temp==ckpt.lock()) check = true; // this will be final
        n=temp->degree;
        if(heap[n]!=nullptr){
            if(heap[n]==temp) {
                temp = temp->right;
                //it = it->right;
                continue;
                
            }
            //auto ckpt = temp->right;
            while(heap[n]!=temp){
                if(heap[n]->key<temp->key){
                    merge(heap[n], temp);
                    temp = heap[n];
                    //std::cout<<temp->child->key<<" is merged"<<std::endl;
                    heap[n] = nullptr;
                    n=temp->degree;
                    if(heap[n]==nullptr) heap[n] = temp;
                    else continue;
                    if(min_node->key>=temp->key) min_node = temp;
                }
                else {
                    merge(temp,heap[n]);
                    //std::cout<<temp->child->key<<" is merged"<<std::endl;
                    heap[n] = nullptr;
                    n=temp->degree;
                    if(heap[n]==nullptr) heap[n] = temp;
                    else continue;
                    if(min_node->key>=temp->key) min_node = temp;
                }
                //std::cout<<"merged: "<<heap[n]->key<<" "<<heap[n]->degree<<std::endl;
                //if(it->parent.lock()) {it = it->parent.lock(); std::cout<<"it is ascended key is "<<it->key<<std::endl;}
            }//end of merging
            //temp = ckpt->left.lock();
            //temp = it->left.lock();
        }
        else {
            heap[n] = temp;
            //n = temp->degree;
            if(min_node->key>temp->key) min_node = temp;
            //std::cout<<"no merge: "<<heap[n]->key<<" "<<heap[n]->degree<<std::endl;
        }
        //std::cout<<"temp key: "<<temp->key<<", min key: "<< min_node->key<<std::endl;
        temp = temp->right;
        //it = it->right;
        //std::cout<<"process end min key is:"<<min_node->key<<std::endl;
        //std::cout<<std::endl;
    }
    //std::cout<<std::endl;
    //std::cout<<std::endl;
    //std::cout<<std::endl;
    delete[] heap;
    temp = nullptr;
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
        x->degree = x->degree + y->degree + 1;
        return;
    }
    y->right = x->child;
    y->left  = x->child->left;
    x->child->right->parent.lock() = nullptr;
    x->child->left.lock()->parent.lock() = nullptr;
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
//    FibonacciHeap<int> heap ;
//    for(int i = 9 ; i > 0 ; --i) {
//        heap.insert(i);
//    }
//    std::cout<<heap.extract_min().has_value()<<std::endl;
//    std::cout<<(heap.get_min_node()==nullptr)<<std::endl;
//    std::cout<<heap.extract_min().has_value()<<std::endl;
//    std::cout<<heap.extract_min().has_value()<<std::endl;
//    std::cout<<heap.extract_min().has_value()<<std::endl;
//    std::cout<<heap.extract_min().has_value()<<std::endl;
//    std::cout<<heap.extract_min().has_value()<<std::endl;
    FibonacciHeap<int> heap;
    std::vector<int> inserted;
    std::vector<int> extracted;
    size_t N = 10;
    // Prepare a random vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<size_t> dis_ins(200,500);
    std::uniform_int_distribution<size_t> dis_ext(100, 200);
    std::uniform_int_distribution<size_t> dis_key(0, 3000);

    for(size_t i = 0 ; i < N; ++i) {
        extracted.clear();

        size_t num_ins = dis_ins(g);
        size_t num_ext = dis_ext(g);

        for(size_t j = 0 ; j < num_ins ; ++j) {
            int v = dis_key(g);
            inserted.emplace_back(v);
            heap.insert(v);
            std::cout<< v <<" is inserted"<<std::endl;
        }

        for(size_t k = 0 ; k < num_ext ; ++k) {
            int ext = heap.extract_min().value();
            extracted.emplace_back(ext);
            std::cout<<ext<<" has been delted"<<std::endl;
            std::cout<<std::endl;
            std::cout<<"%%%"<<std::endl;
            std::cout<<std::endl;
            
        }
        std::sort(inserted.begin(), inserted.end(), std::less<int>());
        for(size_t l = 0 ; l < num_ext ; ++l) {
            if(inserted[l] != extracted[l])
                std::cout<<"no!!"<<std::endl;
        }
        


        std::sort(inserted.begin(), inserted.end(), std::less<int>());
        for(size_t l = 0 ; l < num_ext ; ++l) {
            if(inserted[l] != extracted[l]) std::cout<< "there is problem"<<std::endl;
        }
        inserted = std::vector<int>(inserted.begin() + num_ext, inserted.end());


        // Every root in the root list has a distinct degree value
        
    }

    
    
}
