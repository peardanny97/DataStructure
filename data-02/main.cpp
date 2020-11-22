#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <memory>


template <typename T>
class TreeNode
{
    public:
        T element;
        std::unique_ptr<TreeNode<T>> left;
        std::unique_ptr<TreeNode<T>> right;

        TreeNode<T>(const T& e)
            :element{e}, left{nullptr}, right{nullptr} {}

        ~TreeNode() {}

};


template <typename T>
struct BST
{
    public:
        std::unique_ptr<TreeNode<T>> root = nullptr;

        ~BST() {}

        bool insert(const T& key);
        bool search(const T& key);
        bool remove(const T& key);

    private:
        bool insert(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool search(std::unique_ptr<TreeNode<T>>& t, const T& key);
        bool remove(std::unique_ptr<TreeNode<T>>& t, const T& key);

};

template <typename T>
bool BST<T>::insert(const T& key) {
    return insert(root, key);
}

template <typename T>
bool BST<T>::search(const T& key) {
    return search(root, key);
}

template <typename T>
bool BST<T>::remove(const T& key) {
    return remove(root, key);
}

template <typename T>
bool BST<T>::insert(std::unique_ptr<TreeNode<T>>& t, const T& key) {
    if(t == nullptr) {t= std::make_unique<TreeNode<T>>(key); return true;}
    else if(key == t->element) return false;
    else if(key < t->element) return insert(t->left,key);
    else if(key > t->element) return insert(t->right,key);
    else return false;
    // TODO
    // if insertion fails (i.e. if the key already exists in tree), return false
    // otherwise, return true

}

template <typename T>
bool BST<T>::search(std::unique_ptr<TreeNode<T>>& t, const T& key) {
    
    if(t == nullptr) return false;
    else if(key == t->element) return true;
    else if(key < t->element) return search(t->left,key);
    else if(key > t->element) return search(t->right,key);
    else return false;
    // TODO
    // if key exists in tree, return true
    // otherwise, return false

}

template <typename T>
bool BST<T>::remove(std::unique_ptr<TreeNode<T>>& t, const T& key) {
    if(t == nullptr) return false;
    else if(key == t->element) {
        if(t->left == nullptr && t->right == nullptr){t=nullptr;return true;}
        else if(t->left == nullptr){
            
            t = std::move(t->right);
            
            return true;
            
        }
        else if(t->right == nullptr){
            
            t = std::move(t->left);
            
            return true;
            
        }
        else{
            auto temp = t->left.get();
            
            while(temp ->right != nullptr){
                temp = temp->right.get();
            }
            T t_element = temp->element;
            t->element = t_element;
            
            remove(t->left,t_element);
            
            
            //std::unique_ptr<TreeNode<T>> temp2 = std::move((std::unique_ptr<TreeNode<T>>)temp);
            //temp2 = std::move(temp2->left);
            return true;
        }

    }
    else if(key < t->element) return remove(t->left,key);
    else if(key > t->element) return remove(t->right,key);
    // TODO
    // if key does not exist in tree, return false
    // otherwise, return true
    else return false;
}

int main(){
    BST <int> A;
    A.insert(5);
    A.insert(3);
    A.insert(7);
    A.insert(2);
    A.insert(4);
    A.insert(1);
    A.insert(9);
    A.remove(3);
    std::cout<< (A.search(3))<<std::endl;
    std::cout<<(A.search(1))<<std::endl;
    return 0;
    
}
